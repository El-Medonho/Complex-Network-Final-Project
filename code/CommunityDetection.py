import sys
import os
import networkx as nx

def main():
    language_filter = ""
    if len(sys.argv) == 2:
        language_filter = sys.argv[1].upper()
    elif len(sys.argv) > 2:
        print("Error: Only one language filter argument allowed.")
        sys.exit(1)

    prefix = f"{language_filter}_" if language_filter else "_"
    
    input_dir = "../ExtractedData/"
    output_dir = "../Communities/"
    input_filename = f"{prefix}network.txt"
    output_filename = f"{prefix}communities.txt"
    
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

    print("Running Louvain Community Detection (NetworkX native)...")
    print(" -> This might take a while depending on network size.")
    
    try:
        communities_list = nx.community.louvain_communities(G, weight='weight')
    except AttributeError:
        print("Error: Your NetworkX version is too old. Please run 'pip install networkx --upgrade'")
        sys.exit(1)
    
    num_communities = len(communities_list)
    print(f" -> Detection complete. Found {num_communities} communities.")

    print(f"Saving communities to: {output_path}")
    try:
        with open(output_path, 'w') as f:
            for community_id, community_nodes in enumerate(communities_list):
                for node in community_nodes:
                    f.write(f"{node} {community_id}\n")
    except Exception as e:
        print(f"Error writing to file: {e}")
        sys.exit(1)

    print("Done successfully.")

if __name__ == "__main__":
    main()