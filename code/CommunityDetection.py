import sys
import os
import networkx as nx
import random as rnd
# import nx-cugraph as nx

# Needs to have an argument for which algorithm will be used. 1 for Louvain, anything else for Leiden
# Ex: python3 ./CommunityDetection.py 1 BR 

# Iterations is currently set to 30

def main():
    iterations = 30
    language_filter = ""
    algo = 0
    if len(sys.argv) < 2:
        print("Error: Needs to include which algo will be used.")
        sys.exit(1)
    elif len(sys.argv) >= 2:
        algo = int(sys.argv[1])
    
    if len(sys.argv) == 3:
        language_filter = sys.argv[2].upper()
    elif len(sys.argv) > 3:
        print("Error: Only one language filter argument allowed.")
        sys.exit(1)

    prefix = language_filter if language_filter else "_"
    
    input_dir = "../ExtractedData/"
    output_dir = "../Communities/"
    input_filename = f"{prefix}network.txt"
    output_filename = f"{prefix}communities_leiden.txt"
    if(algo == 1): output_filename = f"{prefix}communities_louvain.txt"

    input_path = os.path.join(input_dir, input_filename)
    output_path = os.path.join(output_dir, output_filename)

    print(f"Reading network from: {input_path}")
    if not os.path.exists(input_path):
        print(f"Error: File {input_path} not found. Run the C++ extractor first.")
        sys.exit(1)

    try:
        G = nx.read_weighted_edgelist(input_path, delimiter=" ", nodetype=str)
    except Exception as e:
        print(f"Error reading graph: {e}")
        sys.exit(1)
    
    print(f" -> Graph loaded. Nodes: {G.number_of_nodes()}, Edges: {G.number_of_edges()}")

    best_modularity = -1e18
    best_partition = G

    while (iterations > 0):
        print("Running louvain/leiden algorithm.")
        
        if(algo == 1):
            communities_list = nx.community.louvain_communities(G, weight='weight', seed=rnd.randint(41, 10000000009), backend="cugraph")
        else:
            communities_list = nx.community.leiden_communities(G, weight='weight', seed=rnd.randint(41, 10000000009), backend="cugraph")
        
        modularity = nx.community.modularity(G, communities_list, weight='weight')
        num_communities = len(communities_list)
        print(f" -> Detection run complete. Found {num_communities} communities. Current modularity was {modularity}")

        if(modularity > best_modularity):
            best_modularity = modularity
            best_partition = communities_list

        if(algo == 1): break
        
        iterations -= 1

    print(f" -> Detection fully complete. Best modularity was {best_modularity}")



    if not os.path.exists(output_dir):
        print(f"Creating directory: {output_dir}")
        os.makedirs(output_dir, exist_ok=True)

    print(f"Saving communities to: {output_path}")
    try:
        with open(output_path, 'w') as f:
            for community_id, community_nodes in enumerate(best_partition):
                for node in community_nodes:
                    f.write(f"{node} {community_id}\n")
    except Exception as e:
        print(f"Error writing to file: {e}")
        sys.exit(1)

    print("Done successfully.")

if __name__ == "__main__":
    main()