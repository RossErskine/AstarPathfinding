# A* Pathfinding

A* pathfinding search is an iterative search that evaluates every search by giving every piece of data to be searched a score $f ( n )$ the data with the lowest score is determined to be closest to the finish by using the formula:

$$\begin{equation} f(n) = g(n) + h(n) \end{equation} $$

where $g ( n )$ records the shortest distance from the start position and $h ( n )$ is the heuristic that estimates the shortest distance to the finish position, as a result, $f ( n )$ estimates the shortest path of moves from the start position to the finish position, passing through n Heineman et al (2016). Mathew (2015) states that the standard heuristic to use for a square grid is the Manhattan distance where the cost function of d is found and can be described as:

$$\begin{equation} d * (dx = abs(node.x – goal.x) + dy = abs(node.y – goal.y)) \end{equation}$$

Harabor and Grastien (2013) and Cormen (2009) both state that Euclidean distance is another heuristic used in A* pathfinding and is
described as:

$$\begin{equation} h(n) = \sqrt{(n . x−goal . x)^2 +(n . y −goal . y)^2} \end{equation}$$ 

However, Patel (2021) and Growing with the web (2016) both state that Euclidean distance should be avoided to reduce the cost of the algorithm, instead use of Euclidean distance squared is described as:

$$\begin{equation} h( n)^2 =(n . x−goal . x)^2 +(n . y −goal. y)^2\end{equation}$$ 

Though it should be noted that the sources Patel (2021) and Growing with the web (2016) are just blog posts yet, to find if they are correct could be interesting. So, the next section will explore how these three heuristics perform on our pathfinding algorithm.


![Screenshot from 2023-07-21 11-28-43](https://github.com/RossErskine/AstarPathfinding/assets/46631932/111e0b19-14c0-4569-bb10-5a65bc59b891)



To perform our tests on our pathfinding algorithm each heuristic was compared against the number of searches needed to find the finish cell that was on the opposite side diagonally on a grid 16 x 16 size, how many cells were needed for the final path and whether the algorithm performed better moving diagonal or not. To make the tests more challenging walls were added, when each cell was created a cell would become a wall with the probability of 0.1, 0.2, 0.3 or 0.4 which would make the tests more difficult, however, the difficulty could not go further than 0.3 on non-
diagonal and 0.4 on diagonal else, the tests would not allow a passable way to the finish. The results show to see (figure 4) that Euclidean distance squared outperformed the other two heuristics quite considerably with Manhattan outperforming Euclidean distance with the least searches on our non-diagonal movement test, all three heuristics found their target in 31 cells.

Unfortunately, our diagonal tests see (figure 5) did not show us significant comparisons perhaps because a random number engine function that generates a uniformly distributed sequence of values and a distribution function that generates a sequence of values was used that gave us the same random number each time the program ran Stroustrup( 2014: p914), which can be a good idea for when testing giving the same testing environment each time. However, this left the diagonal solution unable to be tested properly, further tests could be done where walls were added manually which could take more time but give better results.

![image](https://github.com/RossErskine/AstarPathfinding/assets/46631932/61a874be-5ef9-4578-a576-3f24e6d90cbc)

Mathew (2015) states that the Pathfinding algorithm can be used in many situations and is popular in the gaming industry, in strategy games where many players can be playing in a play area with many obstacles and may be looking for different or similar objects where the size of the heuristic and the cost effect can have large implications in power consumption, memory and overall gameplay.

# references
Growing with the web (2016). A* pathfinding algorithm. https://www.growingwiththeweb.com/2012/06/a-pathfinding-algorithm.html

Harabor, D., & Grastien, A. (2013). An Optimal Any-Angle Pathfinding Algorithm. Proceedings of the International Conference on Automated Planning and Scheduling, 23(1). Retrieved from https://ojs.aaai.org/index.php/ICAPS/article/view/13609

Mathew, G. E. (2015). Direction Based Heuristic for Pathfinding in Video Games. Procedia Computer Science, 47, 262–271. Negnevitsky, M. (2011). Artificial intelligence: a guide to intelligent
systems. (3rd) Harlow: Pearson Education Limited

Patel, A (2021) Heuristics. https://www.growingwiththeweb.com/2012/06/a-pathfinding-algorithm.html

![image](https://github.com/RossErskine/AstarPathfinding/assets/46631932/f053a0b0-45fb-4025-a5b9-f72e31590624)

![image](https://github.com/RossErskine/AstarPathfinding/assets/46631932/81ee6872-91b4-430b-b605-17eb060164e5)

![image](https://github.com/RossErskine/AstarPathfinding/assets/46631932/6bb16546-2306-4b9a-ab52-66a024ed3d0e)

![image](https://github.com/RossErskine/AstarPathfinding/assets/46631932/12f9c0ce-b55d-44e4-8816-a1b4bcd65d79)

![image](https://github.com/RossErskine/AstarPathfinding/assets/46631932/7c67f90c-981d-4bfe-9951-0356cbfcd9b4)






