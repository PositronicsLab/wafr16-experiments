#include <Moby/RCArticulatedBody.h>
#include <Moby/CCD.h>
#include <Moby/CollisionGeometry.h>

using std::vector;
using boost::shared_ptr;
using boost::dynamic_pointer_cast;
using namespace Moby;
using namespace Ravelin;

/// sets a vector to a random value, each in [-1.0, 1.0]
void rand_vec(VectorNd& v)
{
  for (unsigned i=0; i< v.size(); i++)
    v[i] = (double) rand() / RAND_MAX * 2.0 - 1.0;
}

/// Computes a trial
void trial(shared_ptr<RCArticulatedBody> body)
{
  // get the vector of links from the multibody
  const vector<shared_ptr<RigidBodyd> >& links = body->get_links();

  // set the multibody to a random configuration and random velocity
  VectorNd gc, gv;
  const unsigned N_GC = body->num_generalized_coordinates(DynamicBodyd::eSpatial);
  gc.resize(N_GC);
  gv.resize(N_GC);
  rand_vec(gc);
  rand_vec(gv);
  body->set_generalized_coordinates_euler(gc);
  body->set_generalized_velocity(DynamicBodyd::eSpatial, gv);

  // pick a random direction vector
  Vector3d d(GLOBAL);
  d[0] = (double) rand() / RAND_MAX * 2.0 - 1.0;
  d[1] = (double) rand() / RAND_MAX * 2.0 - 1.0;
  d[2] = (double) rand() / RAND_MAX * 2.0 - 1.0;
  d.normalize();

  // get the current pose of a point on the multibody 
  // 1. pick a random, non-base link
  unsigned link = rand() % (links.size()-1) + 1;
  // 2. get the bounding radius from the link
  RigidBodyPtr rb = dynamic_pointer_cast<RigidBody>(links[link]); 
  double rad = rb->geometries.front()->get_geometry()->get_bounding_radius();
  // 3. pick a random point on the bounding radius
  Point3d p(links[link]->get_pose());
  p[0] = (double) rand() / RAND_MAX * 2.0 * rad - rad;
  p[1] = (double) rand() / RAND_MAX * 2.0 * rad - rad;
  p[2] = (double) rand() / RAND_MAX * 2.0 * rad - rad;

  // get the point in the global frame
  Point3d p0 = Pose3d::transform_point(GLOBAL, p);

  // compute the conservative advancement step of the link along d
  double CA_dist = CCD::calc_max_dist(body, rb, d, rad);

  // integrate the multibody by 1.0
  gv *= 1.0;
  gc += gv;
  body->set_generalized_coordinates_euler(gc);

  // get the new pose of the same point on the multibody
  Point3d p0_x = Pose3d::transform_point(GLOBAL, p);
 
  // check the actual distance along d
  double dist = std::fabs((p0_x - p0).dot(d)); 

  // compare the distance with the conservative distance
  std::cerr << (CA_dist - dist) << std::endl;
}
 
extern "C" {

void init(void* separator, const std::map<std::string, BasePtr>& read_map, double t)
{
  shared_ptr<RCArticulatedBody> body;

  for (std::map<std::string, BasePtr>::const_iterator i = read_map.begin();
       i !=read_map.end(); i++)
  {
    if (!body)
      body = boost::dynamic_pointer_cast<RCArticulatedBody>(i->second); 
  }

  srand(time(NULL));

  // run the specified number of trials
  for (unsigned i=0; i< 100000; i++)
    trial(body);

  exit(0);
}

}

