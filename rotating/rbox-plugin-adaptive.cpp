 /*****************************************************************************
 * Moby plugin for setting up and outputting data from the die experiment 
 * Generates the following files during a simulation run:
 * - rke.dat: the kinetic energy of the die
 * - telemetry.box: the position and orientation of the die c.o.m
 * - manifold.change: the virtual time of any contact manifold changes.
 ****************************************************************************/

#include <Moby/TimeSteppingSimulator.h>
#include <Moby/RCArticulatedBody.h>
#include <Moby/GravityForce.h>
#include <Ravelin/Pose3d.h>
#include <Ravelin/Vector3d.h>
#include <Ravelin/VectorNd.h>
#include <fstream>
#include <stdlib.h>

using boost::shared_ptr;
using std::map;
using namespace Ravelin;
using namespace Moby;

Moby::RigidBodyPtr box;
boost::shared_ptr<TimeSteppingSimulator> sim;
boost::shared_ptr<GravityForce> grav;
std::map<sorted_pair<shared_ptr<SingleBodyd> >, unsigned> last_contacts;

/// Computes a random pose, all position values in [-1, 1]
void rand_pose(Pose3d& p)
{
  p.x[0] = (double) rand() / RAND_MAX * 2.0 - 1.0; 
  p.x[1] = (double) rand() / RAND_MAX * 2.0 - 1.0; 
  p.x[2] = (double) rand() / RAND_MAX * 2.0 - 1.0; 
  p.q.x = (double) rand() / RAND_MAX * 2.0 - 1.0; 
  p.q.y = (double) rand() / RAND_MAX * 2.0 - 1.0; 
  p.q.z = (double) rand() / RAND_MAX * 2.0 - 1.0; 
  p.q.w = (double) rand() / RAND_MAX * 2.0 - 1.0; 
  p.q.normalize();
}

/// Gets a random velocity
SVelocityd rand_velocity(shared_ptr<const Pose3d> P)
{
  SVelocityd v;
  v.pose = P;
  for (unsigned i=0; i< 6; i++)
    v[i] = (double) rand() / RAND_MAX * 20.0 - 10.0; 
  return v;
}

// post-contact handling callback
void post_contact_callback(std::vector<Constraint>& constraints, shared_ptr<void> data)
{
  map<sorted_pair<shared_ptr<SingleBodyd> >, unsigned> nc;

  // get the number of contacts between the bodies
  for (unsigned i=0; i< constraints.size(); i++)
    if (constraints[i].constraint_type == Constraint::eContact)
      nc[make_sorted_pair(constraints[i].contact_geom1->get_single_body(), constraints[i].contact_geom2->get_single_body())]++;

  // if the number of contacts change, record a contact manifold time change
  for (map<sorted_pair<shared_ptr<SingleBodyd> >, unsigned>::const_iterator nc_iter = nc.begin(); nc_iter != nc.end(); nc_iter++)
  {
    // see how many contacts are in the current one
    if (last_contacts.find(nc_iter->first) == last_contacts.end() || last_contacts[nc_iter->first] != nc_iter->second)
    {
      std::ofstream out("manifold.change", std::ostream::app);
      out << sim->current_time << std::endl;
      out.close();
      break;
    }
  } 

  // save the new contact state
  last_contacts = nc;
}

// simulator callback
void post_step_callback(Simulator* sim)
{
  // output the sliding velocity at the contact 
  std::ofstream out("rke.dat", std::ostream::app);
  out << sim->current_time << " " << box->calc_kinetic_energy() << std::endl;
  out.close();

  // save the generalized coordinates of the box
  out.open("telemetry.box", std::ostream::app);
  VectorNd q;
  box->get_generalized_coordinates_euler(q);
  out << sim->current_time;
  for (unsigned i=0; i< q.size(); i++)
    out << " " << q[i];
  out << std::endl;
  out.close();
}

/// plugin must be "extern C"
extern "C" {

void init(void* separator, const std::map<std::string, Moby::BasePtr>& read_map, double time)
{
  // wipe out existing files 
  std::ofstream out("rke.dat");
  out.close();
  out.open("telemetry.box");
  out.close();
  out.open("manifold.change");
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
    if (!grav)
      grav = boost::dynamic_pointer_cast<GravityForce>(i->second);
  }

  sim->post_step_callback_fn = &post_step_callback;
  sim->constraint_callback_fn = &post_contact_callback;
  srand(0);

  // set the position, orientation, and velocities to some random values
  Pose3d x = *box->get_pose();
  rand_pose(x);

  // add 2.0 to the initial pose for the box
  x.x[1] += 2.0;
  box->set_pose(x);

  // set random velocity for the box
  shared_ptr<const Pose3d> P = box->get_velocity().pose;
  box->set_velocity(rand_velocity(P));
}
} // end extern C
