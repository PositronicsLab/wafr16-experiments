/*****************************************************************************
 * "Controller" for outputting data from the simple box examples 
 ****************************************************************************/
#include <Moby/TimeSteppingSimulator.h>
#include <Moby/CollisionDetection.h>
#include <Moby/RCArticulatedBody.h>
#include <Moby/GravityForce.h>
#include <Ravelin/Pose3d.h>
#include <Ravelin/Vector3d.h>
#include <Ravelin/VectorNd.h>
#include <fstream>
#include <stdlib.h>

using boost::shared_ptr;
using namespace Ravelin;
using namespace Moby;

Moby::RigidBodyPtr box;
Moby::RigidBodyPtr ground;
boost::shared_ptr<TimeSteppingSimulator> sim;
boost::shared_ptr<GravityForce> grav;

// setup simulator callback
void post_step_callback(Simulator* sim)
{
  // output the sliding velocity at the contact 
  std::ofstream out("ke_implicit.dat", std::ostream::app);
  out << sim->current_time << " " << box->calc_kinetic_energy() << std::endl;
  out.close();
}


//compute the the contact data using 8 vertices
void contact_callback_fn(std::vector<Constraint> e, boost::shared_ptr<void> empty)
{
  boost::shared_ptr<Pose3d> GLOBAL;
  Moby::CollisionGeometryPtr box_cg = *(box->geometries.begin());
  Moby::CollisionGeometryPtr plane_cg = *(ground->geometries.begin());


  std::vector<Point3d> vertices;

  //get all the vertices
  box_cg->get_vertices(vertices);
  
  
  // for all vertices 
  for(std::vector<Point3d>::const_iterator i = vertices.begin(); i != vertices.end(); ++i)
  {
    std::vector<Vector3d> normals; 
    Ravelin::Transform3d wTp = Pose3d::calc_relative_pose((*i).pose , GLOBAL);
    Point3d p_w = wTp.transform_point(*i);
    double dist = p_w.y();
    Constraint n_constraint = CollisionDetection::create_contact(box_cg,plane_cg,p_w,normals[0], dist);
    e.push_back(n_constraint);
  }

}

/// plugin must be "extern C"
extern "C" {

void init(void* separator, const std::map<std::string, Moby::BasePtr>& read_map, double time)
{
  // wipe out existing file 
  std::ofstream out("rke-implicit.dat");
  out.close();

  // get a reference to the TimeSteppingSimulator instance
  for (std::map<std::string, Moby::BasePtr>::const_iterator i = read_map.begin();
       i !=read_map.end(); i++)
  {
    // Find the simulator reference
    if (!sim)
      sim = boost::dynamic_pointer_cast<TimeSteppingSimulator>(i->second);
    if (i->first == "box")
      box = boost::dynamic_pointer_cast<RigidBody>(i->second);
    if (i->first == "ground")
      ground = boost::dynamic_pointer_cast<RigidBody>(i->second);
    if (!grav)
      grav = boost::dynamic_pointer_cast<GravityForce>(i->second);
  }

  sim->post_step_callback_fn = &post_step_callback;

}
} // end extern C
