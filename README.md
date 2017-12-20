# Game_Physics_Independent_Study
A repo to hold everything for my independent study into Game Physics with a addition of soft-body.

Engine Layout:


Soft-Body Physics:
I decided to loosely base my soft-body physics off of springs.
So, my soft-body currently is made for spheres, but can be used for any collision mesh type. I need the calculation of a vector and a point, the collision point and the deformation vector. 
Collision Point - the point of collision between the two objects
Deformation Vector - how much the object should deform at the collision point (in total)
  This is where we use some basic physics. For springs we know F = -kx and in general Sum of F = ma.
  Since, we are using Impulse forces for movement, I do ma = -kx.
  F = -kx translates to the force acting on a spring equals the negative spring constant multiplied by the deformation.
