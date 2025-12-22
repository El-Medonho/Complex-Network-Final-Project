# Analysis of communities in co-acting networks of actors and correlation with movies metadata

Final project for my Complex Networks subject. This project analyses an actor co-star network to detect collaboration communities. It explores the correlation between these communities and film genre, language and actor's age. Uses Louvain and Leiden algorithms for community detection and heuristic methods for correlation.

For more details on the work and analysis, refer to the .ipynb file or load the .html file

This project uses the public dataset from imdb (https://developer.imdb.com/non-commercial-datasets/). To run the algorithms on the graph, i used the NetworkX library with the cu-graph (https://rapids.ai/nx-cugraph/) backend (uses cuda to run Louvain and Leiden faster). The full project is in my github (https://github.com/El-Medonho).

- ExtractData.cpp is used to extract data from the dataset and create more friendly .txt files with the information that was needed in the analysis.

- CommunityDetection.py is used to run Louvain and Leiden community detecting algorithms on the extracted networks.

If you want to run the tests for yourself or increment on this code, here is a guide:

1. Download the following files from the imdb dataset:
    - title.basics.tsv
    - title.principles.tsv
    - title.akas.tsv
    - name.basics.tsv

2. Create a folder called "imdbDataset" inside the root of the project. Move the folders with the files inside this folder.

3. Create two more folders inside the root of the project: "Communities" and "ExtractedData". These will store data from the network that will be extracted by running the cpp and python programs.

4. Compile and run ExtractData.cpp, it accepts up to one argument, that refers to the language of the movies. If no argument is passed, it will get movies from every language. Since I used US and BR as regions for my analysis, you can use these for tests.

5. Run CommunityDetection.py, it needs one argument that refers to the algorithm that will be used (1 for Louvain, anything else for Leiden). It also accepts another argument, referring to the language that will be used. Since I used US and BR as regions for my analysis, you can use these for tests.

6. Now you are free to use and run Analysis.ipynb notebook!

Note: For the age analysis of the actors, I am using 2025 as the year of reference in the notebook. I suggest the user to change this value to the current year.