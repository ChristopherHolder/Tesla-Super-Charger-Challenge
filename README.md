# Autopilot Simulation Challenge 
## An aproximate solution..

In general the main idea is to create a graph for which multiple nodes correspond to each station at different stages of charge.
Then, edges are created from nodes in different stations if they are reachable with the current charges.
After, it just follows regular Dijktra from the start station at top charge and ends at end station at 0 charge, since the previous station should
only charge enough to reach the destination.  
The number of stages is controlled by the PRECISION macro in 'geo.cpp'. As this value approaches INF the more precise the algorithm should be.

When the TEST macro is set to 1 , this initiates some tests.

Previous considerations:
- Originally I was going to use arrays more but to allow  variable number of entries in the csv I went with vectors.
- I took haversine formula from here : https://www.geeksforgeeks.org/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
- I also considered just to speed up program runtime adding concurrency to graph creation.

Date: 06/13/2021