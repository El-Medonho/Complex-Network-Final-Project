#include "bits/stdc++.h"

using namespace std;

/*
Accepts up to one argument. Will filter movies based on region/language passed by argument. 
Ex: "./ExtractData US" will filter movies produced in the US
*/

vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void toUpper(string &s) {
    for(char &c: s) c = toupper(c);
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false), cin.tie(nullptr);

    if(argc > 2){
        cout << "Only one language filter available." << endl;
        return 1;
    }

    string languageFilter = "";
    if(argc == 2) {languageFilter = argv[1];}

    set<string> movie_tconsts;
    map<string, string> movie_genres;
    map<string, string> movie_regions;
    map<string, string> person_birth_year;

    cout << "Accessing title.basics.tsv" << endl;
    ifstream basicsFile("../imdbDataset/title.basics.tsv/title.basics.tsv");
    if (!basicsFile.is_open()) {
        cerr << "Not able to open title.basics.tsv" << endl;
        return 1;
    }

    string line;
    getline(basicsFile, line); 

    // Saves available movies and their genres

    while (getline(basicsFile, line)) {
        int firstTab = line.find('\t');
        int secondTab = line.find('\t', firstTab + 1);
        
        if (firstTab != string::npos && secondTab != string::npos) {
            string tconst = line.substr(0, firstTab);
            string titleType = line.substr(firstTab + 1, secondTab - firstTab - 1);

            if (titleType == "movie") {
                movie_tconsts.insert(tconst);
                
                int currentTab = secondTab;
                for(int i=0; i<6; ++i) {
                    if(currentTab != string::npos)
                        currentTab = line.find('\t', currentTab + 1);
                }
                
                string genres = line.substr(currentTab + 1);
                if (!genres.empty() && genres.back() == '\r') genres.pop_back();
                movie_genres[tconst] = genres;
            }
        }
    }
    basicsFile.close();
    cout << " -> Total Movies found (pre-filter): " << movie_tconsts.size() << endl;


    // if arguments were passed, will now filter movies by language. Will also save regions by movie

    map<string,string> original_title;

    if (!languageFilter.empty()) {
        toUpper(languageFilter);
        cout << "Filtering by language/region: " << languageFilter << endl;
        cout << "Accessing title.akas.tsv" << endl;
        
        ifstream akasFile("../imdbDataset/title.akas.tsv/title.akas.tsv");
        if (!akasFile.is_open()) {
            cerr << "Not able to open title.akas.tsv" << endl;
            return 1;
        }

        set<string> filtered_movies;
        
        getline(akasFile, line);
        
        long long akasRows = 0;
        while (getline(akasFile, line)) {
            int pos = 0; 
            int next_pos;
            
            next_pos = line.find('\t', pos);
            if (next_pos == string::npos) continue;
            string tconst = line.substr(pos, next_pos - pos);

            if(movie_tconsts.find(tconst) == movie_tconsts.end()) continue;
            if (filtered_movies.find(tconst) != filtered_movies.end()) continue;

            pos = next_pos + 1;
            next_pos = line.find('\t', pos);
            
            pos = next_pos + 1;
            next_pos = line.find('\t', pos);
            string title = line.substr(pos, next_pos - pos);

            // gets region
            pos = next_pos + 1;
            next_pos = line.find('\t', pos);
            string region = line.substr(pos, next_pos - pos);

            // gets language
            pos = next_pos + 1; next_pos = line.find('\t', pos);
            string language = line.substr(pos, next_pos - pos);

            // gets types
            pos = next_pos + 1; next_pos = line.find('\t', pos);
            string types = "";
            if (next_pos != string::npos) types = line.substr(pos, next_pos - pos);
            else types = line.substr(pos);
            
            if(types.find("original") != string::npos) {
                original_title[tconst] = title;
                continue;
            }
            
            toUpper(region);
            toUpper(language);

            // Nasty workaround, will not work everytime
            if ((region == languageFilter || language == languageFilter) && original_title[tconst] == title) {
                filtered_movies.insert(tconst);
                
                if (movie_regions.find(tconst) == movie_regions.end()) {
                    string val = "NADA";
                    if (!region.empty() && region != "\\N") val = region;
                    else if (!language.empty() && language != "\\N") val = language;
                    movie_regions[tconst] = val;
                }
            }

            akasRows++;
            if (akasRows % 1000000 == 0) cout << " -> Akas rows processed: " << akasRows << "\r" << flush;
        }
        akasFile.close();
        cout << endl << " -> Movies remaining after filter: " << filtered_movies.size() << endl;

        map<string, string> new_movie_genres = movie_genres;
        for(auto[key, val]: movie_genres){
            if(filtered_movies.find(key) == filtered_movies.end()) new_movie_genres.erase(key);
        }
        movie_genres = new_movie_genres;
        
        movie_tconsts = filtered_movies;
    } 
    else {
        cout << "No filter applied. Accessing title.akas.tsv to extract regions." << endl;
        ifstream akasFile("../imdbDataset/title.akas.tsv/title.akas.tsv");
        if (akasFile.is_open()) {
            getline(akasFile, line);
            long long akasRows = 0;
            while (getline(akasFile, line)) {
                int pos = 0; 
                int next_pos;
                
                next_pos = line.find('\t', pos);
                if (next_pos == string::npos) continue;
                string tconst = line.substr(pos, next_pos - pos);
                
                pos = next_pos + 1;
                next_pos = line.find('\t', pos);
                
                pos = next_pos + 1;
                next_pos = line.find('\t', pos);
                string title = line.substr(pos, next_pos - pos);
                
                // gets region
                pos = next_pos + 1;
                next_pos = line.find('\t', pos);
                string region = line.substr(pos, next_pos - pos);
                
                // gets language
                pos = next_pos + 1; next_pos = line.find('\t', pos);
                string language = line.substr(pos, next_pos - pos);
                
                // gets types
                pos = next_pos + 1; next_pos = line.find('\t', pos);
                string types = "";
                if (next_pos != string::npos) types = line.substr(pos, next_pos - pos);
                else types = line.substr(pos);
                toUpper(region);
                toUpper(language);
                
                // As it seems the original region/ language seems to be the last one that has the same title as the original (i guess)
                if(types.find("original") != string::npos) {
                    original_title[tconst] = title;
                    movie_regions[tconst] = "NADA";
                    continue;
                }

                if(original_title.find(tconst) == original_title.end() || title == original_title[tconst]){
                    string val = "NADA";
                    if (!language.empty() && language != "\\N") val = language;
                    else if (!region.empty() && region != "\\N") val = region;
                    movie_regions[tconst] = val;
                }
                    
                akasRows++;
                if (akasRows % 1000000 == 0) cout << " -> Akas rows processed: " << akasRows << "\r" << flush;
            }
            akasFile.close();
            cout << endl;
        }
    }

    // will now map movies that each person took part in and their birthday's year

    map<string, vector<string>> movie_to_people;
    map<string, vector<string>> person_to_movies;

    cout << "Accessing title.principals.tsv" << endl;
    ifstream principalsFile("../imdbDataset/title.principals.tsv/title.principals.tsv");
    if (!principalsFile.is_open()) {
        cerr << "Not able to open title.principals.tsv" << endl;
        return 1;
    }

    set<string> target_categories = {"actor", "actress", "director", "producer"};

    getline(principalsFile, line);

    long long rowCount = 0;
    while (getline(principalsFile, line)) {
        vector<string> cols = split(line, '\t');
        
        if (cols.size() >= 4) {
            string tconst = cols[0];
            string nconst = cols[2];
            string category = cols[3];

            if (movie_tconsts.find(tconst) != movie_tconsts.end()) {
                if (target_categories.find(category) != target_categories.end()) {
                    movie_to_people[tconst].push_back(nconst);
                    person_to_movies[nconst].push_back(tconst);
                }
            }
        }
        
        rowCount++;
        if (rowCount % 1000000 == 0) cout << " -> Number of rows processed: " << rowCount << "\r" << flush;
    }
    principalsFile.close();
    cout << endl << " -> Map completed" << endl;

    cout << "Accessing name.basics.tsv" << endl;
    ifstream nameFile("../imdbDataset/name.basics.tsv/name.basics.tsv");
    if (nameFile.is_open()) {
        getline(nameFile, line);
        long long nameRows = 0;
        while(getline(nameFile, line)) {
            int firstTab = line.find('\t');
            if (firstTab != string::npos) {
                string nconst = line.substr(0, firstTab);
                
                if (person_to_movies.find(nconst) != person_to_movies.end()) {
                    int secondTab = line.find('\t', firstTab + 1);
                    int thirdTab = line.find('\t', secondTab + 1);
                    if (secondTab != string::npos && thirdTab != string::npos) {
                        string bYear = line.substr(secondTab + 1, thirdTab - secondTab - 1);
                        if(bYear == "\\N") bYear = "NADA";
                        person_birth_year[nconst] = bYear;
                    }
                }
            }
            nameRows++;
            if (nameRows % 1000000 == 0) cout << " -> Name rows processed: " << nameRows << "\r" << flush;
        }
        nameFile.close();
        cout << endl;
    } else {
        cerr << "Warning: Could not open name.basics.tsv" << endl;
    }

    // Finished processing data. Will now start to build text files

    cout << "Building network.txt" << endl;
    
    map<string, int> edges;

    for (auto const& [tconst, people] : movie_to_people) {
        if (people.size() < 2) continue;

        vector<string> unique_people = people;
        sort(unique_people.begin(), unique_people.end());
        unique_people.erase(unique(unique_people.begin(), unique_people.end()), unique_people.end());

        for (int i = 0; i < unique_people.size(); ++i) {
            for (int j = i + 1; j < unique_people.size(); ++j) {
                string p1 = unique_people[i];
                string p2 = unique_people[j];
                
                string key = p1 + " " + p2;
                edges[key]++;
            }
        }
    }

    string prefix = "../ExtractedData/" + (languageFilter.empty() ? "_" : languageFilter);

    ofstream networkFile(prefix + "network.txt");
    for (auto const& [key, weight] : edges) {
        networkFile << key << " " << weight << "\n";
    }
    networkFile.close();

    cout << "Building moviesByPerson.txt" << endl;
    ofstream personFile(prefix + "moviesbyperson.txt");
    
    for (auto const& [nconst, movies] : person_to_movies) {
        personFile << nconst << " " << movies.size();
        for (const auto& movie : movies) {
            personFile << " " << movie;
        }
        personFile << "\n";
    }
    personFile.close();

    cout << "Building peopleByBirthYear.txt" << endl;
    ofstream birthFile(prefix + "peopleByBirthYear.txt");
    for (auto const& [nconst, year] : person_birth_year) {
        birthFile << nconst << " " << year << "\n";
    }
    birthFile.close();

    cout << "Building genresByMovie.txt" << endl;
    ofstream genreFile(prefix + "genresByMovie.txt");
    for (const auto& tconst : movie_tconsts) {
        string g = (movie_genres.count(tconst)) ? movie_genres[tconst] : "\\N";
        genreFile << tconst << " " << g << "\n";
    }
    genreFile.close();

    cout << "Building regionByMovie.txt" << endl;
    ofstream regionFile(prefix + "regionByMovie.txt");
    for (const auto& tconst : movie_tconsts) {
        string r = movie_regions[tconst];
        regionFile << tconst << " " << r << "\n";
    }
    regionFile.close();

    cout << "Data was extracted succsesfully" << endl;

    return 0;
}