dsaps assignment 1

Q1

1. how to execute seamcarving.cpp == ./a.out <image.jpeg> <height> <width>
2. will make a output file name final.jpeg in current directory
3. energy function calculated using Vec3b to store each pixel value relative  to    neighbouring pixels.
4.wraped around edges
5. used energy to calculate dp array which calculates and stores the minimum path taken in path array
6. dp array checks its upper 3 pixels for calculation
7. after getting path made new Mat output , so that i can copy all pixels except  min path 
8. do this for (input.rows-height) number of times, and input.cols-width to get final img

Q2

   use ./a.out then n, q (n words) (q quries)
1. made node, trie class
2. defined functions to link nodes to root using a array[26] of node pointers 
3. a letter is present if its 'a'-i index is not nullptr we can retrieve that letter using 'a'+i
4. uses flag to know if word is valid  
5. uses charr[i]!=nullptr to check word end
6. spellcheck function
    checks if charr[i] is not null and 'a'+i is in our input word
7. autocorrect fun
    uses same dp calculation logic as longest sub string problem with some modifications   
8. autocomplete fun
    traverse the minimum char value path till position->charr[i]=nullptr;  


Q3

use ./a.out then n , then n queries, for flag 0 takes input and updates, for flag 1 immediately prints current silhoutte 
1. uses 2d array to store upto 10000 points 
2. banner adds input point along with its height to sil[][] array
3. getidx tells where the new point should be placed using binary search
4. skyline function checks if points in sil[][ ] form a silhoutte
    by keeping ony the points where height changes 


        int ida=getidx(a, 0,bsize-1);
    int basea=(ida>0)? sil[ida-1][1] : 0;
    banner(a,basea);

    int idb=getidx(b, 0,bsize-1);
    int baseb=(idb>0)? sil[idb-1][1] : 0;
    banner(b,baseb);
