/*********************************************************************************
Copyright (C) 2014 Adrian Jarabo (ajarabo@unizar.es)
Copyright (C) 2014 Diego Gutierrez (diegog@unizar.es)
All rights reserved.

This is an educational Ray Tracer developed for the course 'Informatica Grafica'
(Computer Graphics) tought at Universidad de Zaragoza (Spain). As such, it does not 
intend to be fast or general, but just to provide an educational tool for undergraduate
students. 

This software is provided as is, and any express or implied warranties are disclaimed.
In no event shall copyright holders be liable for any damage.
**********************************************************************************/
#include "PhotonMapping.h"
#include "World.h"
#include "Intersection.h"
#include "Ray.h"
#include "BSDF.h"

//*********************************************************************
// Compute the photons by tracing the Ray 'r' from the light source
// through the scene, and by storing the intersections with matter
// in the lists 'xx_photons', storing the diffuse (global) and caustic
// photons respectively. For efficiency, both are computed at the same
// time, since computing them separately would result into a lost of
// several samples marked as caustic or diffuse.
// Same goes with the boolean 'direct', that specifies if direct 
// photons (from light to surface) are being stored or not. 
// The initial traced photon has energy defined by the tristimulus
// 'p', that accounts for the emitted power of the light source.
// The function will return true when there are more photons (caustic
// or diffuse) to be shot, and false otherwise.
//---------------------------------------------------------------------
bool PhotonMapping::trace_ray(const Ray& r, const Vector3 &p, 
			   std::list<Photon> &global_photons, std::list<Photon> &caustic_photons, bool direct)
{

	//Check if max number of shots done...
	if( ++m_nb_current_shots > m_max_nb_shots )
	{
		return false;
	}
	
	// Compute irradiance photon's energy
	Vector3 energy(p);
	
	Ray photon_ray(r);
	photon_ray.shift();

	bool is_caustic_particle = false;

	//Iterate the path
	while(1)
	{
		// Throw ray and update current_it
		Intersection it;
		world->first_intersection(photon_ray, it);

		if( !it.did_hit() )
			break;

		//Check if has hit a delta material...
		if( it.intersected()->material()->is_delta() )
		{
			// If delta material, then is caustic...
			// Don't store the photon!
			is_caustic_particle = true;
		}
		else if (photon_ray.get_level() > 0 || direct)
		{
			//If non-delta material, store the photon!
			if( is_caustic_particle )	
			{				
				//If caustic particle, store in caustics
				if( caustic_photons.size() < m_nb_caustic_photons )
					caustic_photons.push_back( Photon(it.get_position(), photon_ray.get_direction(), energy ));
			}
			else						
			{
				//If non-caustic particle, store in global
				if( global_photons.size() < m_nb_global_photons )
					global_photons.push_back( Photon(it.get_position(), photon_ray.get_direction(), energy ));
			}
			is_caustic_particle = false;
		}	
		
		Real pdf;

		Vector3 surf_albedo = it.intersected()->material()->get_albedo(it);
		Real avg_surf_albedo = surf_albedo.avg();

		Real epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);
		while (epsilon2 < 0.)
			epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);
		
		if (epsilon2 > avg_surf_albedo || photon_ray.get_level() > 20 ) 
			break;
			
		// Random walk's next step
		// Get sampled direction plus pdf, and update attenuation
		it.intersected()->material()->get_outgoing_sample_ray(it, photon_ray, pdf );

		// Shade...
		energy = energy*surf_albedo;
		if( !it.intersected()->material()->is_delta() )
			energy *= dot_abs(it.get_normal(), photon_ray.get_direction())/3.14159;		

		energy = energy /(pdf*avg_surf_albedo);
	}
	
	if( caustic_photons.size() == m_nb_caustic_photons && 
		global_photons.size() == m_nb_global_photons )
	{
		m_max_nb_shots = m_nb_current_shots-1;
		return false;
	}

	return true;
}

//*********************************************************************
// Takes a Vector3 as argument and returns a std::vector<T> with the
// original data.
// Type MUST be convertable to float
//---------------------------------------------------------------------
template<typename T>
std::vector<T> transform_vector(const Vector3 &original){
	std::vector<T> new_vector(3);
	for(int i = 0; i < 3; i++){
		new_vector[i] = original.data[i];
	}
	return new_vector;
}

//*********************************************************************
// TODO: Implement the preprocess step of photon mapping,
// where the photons are traced through the scene. To do it,
// you need to follow these steps for each shoot:
//  1 - Sample a world's light source in the scene to create
//		the initial direct photon from the light source.
//	2 - Trace the photon through the scene storing the inter-
//		sections between the photons and matter. You can use
//		the function 'trace_ray' for this purpose.
//	3 - Finally, once all the photons have been shot, you'll
//		need to build the photon maps, that will be used later
//		for rendering. 
//		NOTE: Careful with function
//---------------------------------------------------------------------
void PhotonMapping::preprocess()
{
	m_global_map.clear();
	m_caustics_map.clear();
	
	float inclination, azimuth;
	Ray r;
	const int no_lights = world->nb_lights();
	Vector3 p, d, intensities;
	int idx = 0;
	
	std::list<Photon> global_photons, caustic_photons;
	std::vector<float> position;
	do{	//Repeats until there are no more photons to shoot
		
		//Light's position
		p = world->light(idx).get_position();
		//Light's intensity
		intensities = world->light(idx).get_intensities();
		
		//Cosine sample direction
		inclination = acosf(1-2*static_cast<Real>(rand())/static_cast<Real>(RAND_MAX));
		azimuth = 2.0 * M_PI * static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);
		d = Vector3(sinf(inclination)*cosf(azimuth), sinf(inclination)*sinf(azimuth), cosf(inclination));

		//Create the ray with the origin on the light's position, and with the sampled direction
		r = Ray(p, d);

		idx = (idx + 1) % no_lights;
		
	} while (trace_ray(r, intensities, global_photons, caustic_photons, true));
	
	//We multiply the flux of every photon to 4*PI since 1/(4*PI) is the probability of
	//any direction for the photon 
	//We divide the flux of every photon by the number of photons since the sum of the fluxes
	// has to match the energy of the original light

	for(Photon p : global_photons){
		p.flux = p.flux * 4 * M_PI / global_photons.size();
		position = transform_vector<float>(p.position);
		m_global_map.store(position, p);
	}
	for(Photon p : caustic_photons){
		p.flux = p.flux * 4 * M_PI / global_photons.size();
		position = transform_vector<float>(p.position);
		m_caustics_map.store(position, p);
	}
	if(global_photons.size() > 0){
		m_global_map.balance();
	}
	if(global_photons.size() > 0){
		m_caustics_map.balance();
	}
	
}

//*********************************************************************
// TODO: Implement the function that computes the rendering equation 
// using radiance estimation with photon mapping, using the photon
// maps computed as a proprocess. Note that you will need to handle
// both direct and global illumination, together with recursive the 
// recursive evaluation of delta materials. For an optimal implemen-
// tation you should be able to do it iteratively.
// In principle, the class is prepared to perform radiance estimation
// using k-nearest neighbors ('m_nb_photons') to define the bandwidth
// of the kernel.
//---------------------------------------------------------------------
Vector3 PhotonMapping::shade(Intersection &it0)const
{
	Vector3 L(0);
	Intersection it(it0);

	Ray path;
	Real pdf;
	
	//Based on function trace_ray
	for(int iterations = 0; iterations < 20; iterations++){
		Intersection it_path(it);

		if(!it_path.did_hit() || !it.intersected()->material()->is_delta())
			break;
		else
			cout << "uwu";

		it.intersected()->material()->get_outgoing_sample_ray(it, path, pdf);
		path.shift();
		world->first_intersection(path, it);
		

		L = it.intersected()->material()->get_albedo(it) * L;
	}

	std::vector<const KDTree<Photon, 3>::Node*> gl_node, ca_node;
	Real max_distance;
	if(!m_global_map.is_empty()){
		m_global_map.find(transform_vector<Real>(it.get_position()), m_nb_photons, gl_node, max_distance);
		for(auto n : gl_node){
			L += it.intersected()->material()->get_albedo(it) * n->data().flux / (M_PI * max_distance * max_distance);
		}
	}
	if(!m_caustics_map.is_empty()){
		m_caustics_map.find(transform_vector<Real>(it.get_position()), m_nb_photons, ca_node, max_distance);
		for(auto n : ca_node){
			L += it.intersected()->material()->get_albedo(it) * n->data().flux / (M_PI * max_distance * max_distance);
		}
	}

	





	


	//**********************************************************************
	// The following piece of code is included here for two reasons: first
	// it works as a 'hello world' code to check that everthing compiles 
	// just fine, and second, to illustrate some of the functions that you 
	// will need when doing the work. Goes without saying: remove the 
	// pieces of code that you won't be using.
	//
	unsigned int debug_mode = 0;

	switch (debug_mode)
	{
	case 1:
		// ----------------------------------------------------------------
		// Display Albedo Only
		L = it.intersected()->material()->get_albedo(it);
		break;
	case 2:
		// ----------------------------------------------------------------
		// Display Normal Buffer
		L = it.get_normal();
		break;
	case 3:
		// ----------------------------------------------------------------
		// Display whether the material is specular (or refractive) 
		L = Vector3(it.intersected()->material()->is_delta());
		break;

	case 4:
		// ----------------------------------------------------------------
		// Display incoming illumination from light(0)
		L = world->light(0).get_incoming_light(it.get_position());
		break;

	case 5:
		// ----------------------------------------------------------------
		// Display incoming direction from light(0)
		L = world->light(0).get_incoming_direction(it.get_position());
		break;

	case 6:
		// ----------------------------------------------------------------
		// Check Visibility from light(0)
		if (world->light(0).is_visible(it.get_position()))
			L = Vector3(1.);
		break;
	}
	// End of exampled code
	//**********************************************************************

	return L;
}