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
// Returns the maximum of a Vector3
//---------------------------------------------------------------------
Real max(Vector3 v){
	return std::max({v.getComponent(0), v.getComponent(1), v.getComponent(2)});
}

//*********************************************************************
// Returns a vector with the accumulated probabilities for every light
// in the world
//---------------------------------------------------------------------
std::vector<Real> get_lights_probabilities(World* world){
	int no_lights = world->nb_lights();
	std::vector<Real> acc_prob(no_lights);
	float sum = 0;

	for(int i = 0; i < no_lights; i++){
		sum += max(world->light(i).get_intensities());
	}

	acc_prob[0] = max(world->light(0).get_intensities()) / sum;
	for(int i = 1; i < no_lights; i++){
		acc_prob[i] = acc_prob[i-1] + (max(world->light(i).get_intensities()) / sum);
	}
	return acc_prob;
}

//*********************************************************************
// Returns a random integer with a probability given by the accumulated
// probabilities vector
//---------------------------------------------------------------------
int russian_roulette(std::vector<Real> acc_prob){
	Real r = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);
	int i = 0;

	while(r > acc_prob[0]){
		i++;
	}
	return i;
}

//*********************************************************************
// Preprocess step of photon mapping, where the photons are traced 
// through the scene. To do it, it follows these steps for each shoot:
//  1 - Samples a world's light source in the scene to create
//		the initial direct photon from the light source.
//	2 - Traces the photon through the scene storing the inter-
//		sections between the photons and matter. Uses
//		the function 'trace_ray' for this purpose.
//	3 - Finally, once all the photons have been shot, it
//		builds the photon maps, that will be used later
//		for rendering.
//---------------------------------------------------------------------
void PhotonMapping::preprocess()
{
	float inclination, azimuth;
	Ray r;
	const int no_lights = world->nb_lights();
	Vector3 p, d, intensities;
	int idx;

	Real current_prob;
	std::vector<Real> probs = get_lights_probabilities(world);
	//Vector with the list of Photons in which each list with index i has the photons corresponding to light i 
	std::vector<std::list<Photon> > global_photons(no_lights), caustic_photons(no_lights);
	std::vector<float> position;

	do{	//Repeats until there are no more photons to shoot
		
		//Importance Sampling
		idx = russian_roulette(probs);

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
		
	} while (trace_ray(r, intensities, global_photons[idx], caustic_photons[idx], !m_raytraced_direct));
	
	//We multiply the flux of every photon to 4*PI since 1/(4*PI) is the probability of
	//any direction for the photon 
	//We divide the flux of every photon by the number of photons since the sum of the fluxes
	// has to match the energy of the original light
	for(int i = 0; i < no_lights; i++){
		if(i==0) current_prob = probs[0];
		else current_prob = probs[i] - probs[i-1];
		if(global_photons[i].size() > 0){
			for(Photon p : global_photons[i]){
				p.flux = p.flux * 4 * M_PI / (global_photons[i].size() * current_prob);
				position = transform_vector<float>(p.position);
				m_global_map.store(position, p);
			}
			m_global_map.balance();
		}
		if(caustic_photons[i].size() > 0){
			for(Photon p : caustic_photons[i]){
				p.flux = p.flux * 4 * M_PI / (caustic_photons[i].size() * current_prob);
				position = transform_vector<float>(p.position);
				m_caustics_map.store(position, p);
			}
			m_caustics_map.balance();
		}
	}
}

//*********************************************************************
// Function that computes the rendering equation 
// using radiance estimation with photon mapping, using the photon
// maps computed as a proprocess. 
// Handles both direct and global illumination, together with recursive the 
// recursive evaluation of delta materials iteratively.
// Uses k-nearest neighbors ('m_nb_photons') to define the bandwidth
// of the kernel.
//---------------------------------------------------------------------
Vector3 PhotonMapping::shade(Intersection &it0)const
{
	Vector3 L(0);
	Vector3 W(1);
	Intersection it(it0);

	Ray path;
	Real pdf;

	if(m_raytraced_direct && !it.intersected()->material()->is_delta()){
		//Since trace_ray is not storing direct ilumination's photons, we need to trace shadow rays to the lights to get their contribution
		int no_lights = world->nb_lights();
		for(int i = 0; i < no_lights; i++){
			if(world->light(i).is_visible(it.get_position())){
				float distance2 = (world->light(i).get_position() - it.get_position()).length2();
				auto dot_product_result = dot_abs(world->light(i).get_incoming_direction(it.get_position()).normalize(), it.get_normal());
				auto brdf = it.intersected()->material()->get_albedo(it) / M_PI;
				L += world->light(i).get_incoming_light(it.get_position()) * brdf *  dot_product_result / distance2;
			}
		}
	}
	
	//Based on function trace_ray
	for(int iterations = 0; iterations < 20; iterations++){
		Intersection it_path(it);

		if(!it_path.did_hit() || !it.intersected()->material()->is_delta())
			break;

		it.intersected()->material()->get_outgoing_sample_ray(it, path, pdf);
		path.shift();
		world->first_intersection(path, it);
		W = W * it.intersected()->material()->get_albedo(it);
	}

	//Since we're taking samples in a circle, we have to divide the energy by the area of the circle (pi*r^2)
	//Since these are all lambertian materials, we divide by pi (brdf_lambertian=k_lambertian/pi)
	std::vector<const KDTree<Photon, 3>::Node*> gl_node, ca_node;
	Real max_distance;
	if(!m_global_map.is_empty()){
		m_global_map.find(transform_vector<Real>(it.get_position()), m_nb_photons, gl_node, max_distance);
		for(auto n : gl_node){
			L += it.intersected()->material()->get_albedo(it) * n->data().flux / (M_PI * M_PI * max_distance * max_distance);
		}
	}
	if(!m_caustics_map.is_empty()){
		m_caustics_map.find(transform_vector<Real>(it.get_position()), m_nb_photons, ca_node, max_distance);
		for(auto n : ca_node){
			L += it.intersected()->material()->get_albedo(it) * n->data().flux / (M_PI * M_PI * max_distance * max_distance);
		}
	}

	L = L*W;

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
	
	case 7:
		// ----------------------------------------------------------------
		// Reflect and refract until a diffuse surface is found, then show its albedo...
		int nb_bounces = 0;
		// MAX_NB_BOUNCES defined in ./SmallRT/include/globals.h
		while( it.intersected()->material()->is_delta() && ++nb_bounces < MAX_NB_BOUNCES)
		{
			Ray r; float pdf;
			it.intersected()->material()->get_outgoing_sample_ray(it, r, pdf );		
			W = W * it.intersected()->material()->get_albedo(it)/pdf;
			
			r.shift();
			world->first_intersection(r, it);
		}
		L = it.intersected()->material()->get_albedo(it);
	// End of exampled code
	//**********************************************************************
	}

	return L;
}