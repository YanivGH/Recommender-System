Comments:
1. Open the solution in VS 2012 or above.

2. You can make changes in the 'DATABASE_SIZE' (it's a '#define" in the 'DMCPPE.h' file) and compile to see results for verious sizes.

3. The results are presented in the dos prompt, including the run time.

4. I run it on an Intel i5-3470 CPU @ 3.2GHz, GB RAM, 4 core, Win 7, 64 bit.

5. If you compile the solution in "Release mode" the run-time for 'DATABASE_SIZE = 10M' is 1.06 [sec].

6. If you compile the solution in "Debug mode" the run-time for 'DATABASE_SIZE = 10M' is 4.8 [sec].

7. You can test it again with the same dos prompt (until you press ESC).

8. The recommendations are the MOST accurate, i.e. the 10 MOST similar (not 10 'reasonably similar').

9. As the algorithm go over the datastore to look for the most similar entries, I noticed that after a few milions of them, there are only a few updates that can be made to the 10 best that have 
already been found at that point (this is due to the nature of this data). I also notice that after a few milions, any new entry that would enter the top 10 bears only a tiny difference compared to that of
the entry that it would just replace in the top 10. So in fact, I can make the algorithm stop the search after ~ 2M entires and get 10 reasonably similar entries as requested (but not the 10 MOST as I do 
without this small compromise). And going over only 2M entris - the run time is 1 [sec] even in "debug mode" (and 200m [sec] in "Release mode").

10. What I did is a type of 'knn' (k=10) where I'm looking for the top nearest neighbors. Since there wasn't a runtime issue up to a dataset size of 10M entries (and as mentioned above it's suffice to 
search only in the first few milions eaven for a 100M database) then 'knn' worked well even though it's not the most efficient classification/similarity search algorithm. 
Nevertheless, had there was a real time problem, I could have used the 'kdtree' algorithm (build it after database initialization, before query) and use it to reduce the runtime.

11. A note about the sort part during the search: In the search I go thru all the entries and calculate the distance from the entry to the query. I then compare this distance to the highest distance that currently exist in the current "top best" list
(see explanations in the code). When any new entry that gets to enther the top best list, then I sort the list according to their distances (from the query) in order to get the highest one for
the consequent entries in the database. The sort is done NOLY when a new entry gets to enther the top best list (this new entry replaces some other entry). 
But in time there will be less and less sorts since the top best list will contain small distances already - and hence the sort part will become very negligible in the run-time. for example, 
for a 'DatabaseSize' = 2M, the sort was called for only 0.007% of the times.

12. Another way to reduce the run-time (which I didn't do) is to pre-cluster the database into 2M (for example) clusters, so for a 100M database, each such cluster would contain ~50 entries 
(which are very similar to each other). The clustering technique can be as simple as 'kmeans' (no need for more complicated clusting in this problem). And now I would chose one entry from each 
cluster (say the one that is closest to the middle of the cluster) and this entry would represent the entire cluster and would actually replace all of the other entries in its clustter. So now instead of
 working with 100M entris, I would work with only 2M entries - which is much faster. I didn't implement it since The previos methods I meantion (stoping the run after 2M entries) already got to 1 sec).
Another note is that the clustering above (e.g. kmeans) is done only once after the initialization of the database, so it doesn't affect the query run-time.

13. The code contains explanations of variables meaning, values, ranges. The verious choices are justified in the code for all the random sampling (and at some places possible improvements that weren't 
required). There are web links to source files or theoretical explanations where I thought might be required.

14. Regarding the similarity between the query entry and the database entries - I initially thought you wanted that the similarity would be based also on the distance between names of the people 
(though it doesn't make sense, but still). Therefore I implemented also the part that measure distances between names as part of the similarity using 'hamming distance" ("Levenshtein distance" is better 
but consumes more runtime). When I relized you aren't looking for that - I put it in comment and don't use it

15. I used "Haversine formula" to calculate distance between to points on the earth.



