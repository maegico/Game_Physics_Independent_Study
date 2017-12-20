# Game_Physics_Independent_Study
A repo to hold everything for my independent study into Game Physics with a addition of soft-body.

Physics System Layout:
So, I create a collision mesh on start up for each mesh that says to in the modified obj.
  This is not a great way to do it.
After that each Entity has a collider on it, but it does not have to be in use.
I Batch all Entities to the Collision system, which sorts them when they are batched.
Based on this sorting, the system runs through all collision checks and handles them accordingly.
  Currently, there is AABB vs AABB, OBB vs OBB, and Sphere vs Sphere.

Soft-Body Physics:
I decided to loosely base my soft-body physics off of springs.
So, my soft-body currently is made for spheres, but can be used for any collision mesh type. I need the calculation of a vector and a point, the collision point and the deformation vector. 
Collision Point - the point of collision between the two objects
Deformation Vector - how much the object should deform at the collision point (in total)
  This is where we use some basic physics. For springs we know F = -kx and in general Sum of F = ma.
    Since, we are using Impulse forces for movement, I do ma = -kx.
    F = -kx translates to the force acting on a spring equals the negative spring constant multiplied by the deformation.
    For my example I used -1 as k, as that seemed to work.
After calculating those two variables, send them to the vertex shader.
I find the distance between the current vertex and the collision point.
After that I put that through an equation and use it as a scale factor for the deformation vector.
  I use the equation distance = (distance + 1) / (distance * distance * 10).
Finally, I add that to the vertex position, before multiplying the vertex by the MVP matrix.
