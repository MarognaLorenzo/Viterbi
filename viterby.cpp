#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include <unordered_map>
#include <limits>

using namespace std;

typedef unordered_map<string, unordered_map<string, float>> TransitionTable;
typedef unordered_map<string, unordered_map<string, float>> EmissionTable;

class Node {
public:
    string word;
    string tag;
    float emission;
    Node* solution;
    float current_value;
    Node (){};
    void find_best(vector<Node>& prevs, TransitionTable & transition){
        // Use a very small initial value so we can handle scores < 0 if needed
        float max = numeric_limits<float>::lowest();
        Node* best = nullptr;
        for(auto &prev_node: prevs){
            float score = prev_node.current_value * transition[prev_node.tag][tag];
            if(score > max){
                max = score;
                best = &prev_node; // take address of element inside prevs (which should be a layer stored in graph)
            }
        }
        solution = best;
        current_value = best? emission * max : 0;
    }
};

class Viterbi {
public:
    Viterbi(){}
    vector<string> tag(
        vector<string> sentence,
        vector<string> tags,
        EmissionTable emission,
        unordered_map<string, float> initialization,
        TransitionTable transition
    ){

        vector<vector<Node>> graph;
        graph.reserve(sentence.size());
        for (string word: sentence){
            vector<Node> layer;
            layer.reserve(tags.size());
            for(string tag: tags){
                Node n;
                n.emission = emission[word][tag];
                n.word = word,
                n.tag = tag;
                // compute current value
                if(graph.empty()){
                    n.current_value = n.emission * initialization[n.tag];
                    n.solution = nullptr;
                } else {
                    // find_best needs to take addresses of nodes in previous layer; pass previous layer by reference
                    n.find_best(graph.back(), transition);
                }
                layer.push_back(n);
            }
            graph.push_back(layer);
        }

        // find best_end and build solution
        vector<string> solution(sentence.size());
        float maxv = numeric_limits<float>::lowest();
        Node* best_node = nullptr;
        for(auto &node: graph.back()){
            if(node.current_value > maxv){
                maxv = node.current_value;
                best_node = &node;
            }
        }

        int idx = (int)graph.size()-1;
        while(best_node){
            solution[idx] = best_node->tag;
            if(best_node->solution == nullptr) break;
            best_node = best_node->solution;
            idx--;
        }
        return solution;
    }
};

int main(){
    Viterbi v = Viterbi();


    vector<string> sentence = {"iron","shaped","cloth"};
    vector<string> tags = {"verb", "noun", "adj"};
    unordered_map<string, unordered_map<string, float>> emission = 
        { {"iron", {
            {"adj", 0},
            {"verb",2},
            {"noun",3}}},
          {"shaped", {
            {"adj", 2},
            {"verb",3},
            {"noun",0}}},
          {"cloth", {
            {"adj", 0},
            {"verb",0},
            {"noun",1}}
        }};
    unordered_map<string, float> initialization = {
            {"verb", 1},
            {"noun", 2},
            {"adj", 3}
        };
    unordered_map<string, unordered_map<string, float>> transition = 
        { {"adj", {
            {"verb",0},
            {"adj", 0},
            {"noun",5}}},
          {"verb", {
            {"verb", 1},
            {"adj", 2},
            {"noun",2}}},
          {"noun", {
            {"adj", 1},
            {"verb", 2},
            {"noun",0}}
        }};
    
    vector<string> result = v.tag(
        sentence,
        tags,
        emission,
        initialization,
        transition
    );
    cout << "Inserted Sentence:" << endl;
    for(auto word: sentence) cout << word << " ";
    cout << endl << endl;

    cout << "Corresponding tagging:" << endl;
    for(auto el: result) cout << el << " ";
    cout << endl;
    return 0;
}