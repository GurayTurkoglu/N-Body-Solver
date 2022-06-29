# N-Body-Solver
Iteratively calculates positions of celestial objects using Newtonian mechanics. 

2 files should be created to run N-Body Simulation. First file should contain objects and their parameters and second file should contain maneuever parameters.

Syntax for objects file (each line represents an object):
#mass/radius/position_x/position_y/initial_velocity_x/initial_velocity_y;

Syntax for maneuever file (each line represents a maneuever):
#object_id/maneuever_time/maneuever_velocity_x/maneuever_velocity_y;

object_id represents line in objects file. Velocity,position,radius,mass parameters are defined using metric system.

Example: Earth-Moon N-Body Physics Calculation

Earth-Moon mass: 5.972*10^24kg / 7.347*10^22kg         
Earth-Moon Distance from center: 384,405,000m

Earth-Moon radius: 6371000m / 1737000m                 
Moon mean orbital velocity: 1022m/s

Objects File Syntax:

#7.3476e22/1737000/0/384405000/-1022/0;

#5.972e24/6371000/0/0/0/0;

Maneuevers File Syntax:

#1/0/0/0;

#0/0/0/0;

If there are no extra maneuevers, velocity and time parameters on maneuever file should be 0 for each object id.
