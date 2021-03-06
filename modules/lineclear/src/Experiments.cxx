#include "lineclear/CGALTypes.h"
#include "lineclear/Environment.h"
#include "lineclear/ChoiceTree.h"

#include "utilities/stringutils.h"
#include "utilities/Yaml_Config.h"

#include "gui/viewer.h"

#include <iostream>

using std::cout;
using std::endl;
using std::list;
using namespace lineclear;

Viewer *viewer = NULL;

int main(int argc, char **argv) {

  // ====================================
  // = Generate polygon and Environment =
  // ====================================

  string fname_yaml = "../maps/default.yaml";
  Yaml_Config::load_yaml_file_into_param(fname_yaml.c_str());
  int n_polies; 
  srand(time(NULL));
  for (int exp_run = 0; exp_run < 10; exp_run++ ) {
    std::cout << " EXP RUN " << exp_run << std::endl;
    for ( n_polies = 400; n_polies <= 400; n_polies=n_polies+50 ) {
      std::cout << " n_polies " << n_polies << std::endl;
      string basefilename = random_string(20);

      // Generating a random polygon
      polygonization::Polygon_Environment *_pol;
      _pol = new polygonization::Polygon_Environment(
          n_polies);
      string poly_file = basefilename + "_random.poly";
      _pol->save_to_file(poly_file);

      lineclear::Environment *_env;
      _env = new lineclear::Environment(_pol->master_polygon);

      lineclear::ChoiceTree *_ct;
      _ct = new lineclear::ChoiceTree(_env, nullptr, _pol);
      _ct->init_choice_tree();
      _ct->save_to_file(basefilename + ".ct");

      int first_obstacle;
      std::list<int> obstacle_sequence;
      obstacle_sequence = _ct->get_optimal_obstacle_sequence(first_obstacle);
      obstacle_sequence.push_front(first_obstacle);
      int optimal_cost = _ct->get_optimal_cost();
      M_INFO1("OPTIMAL COST %d \n", optimal_cost);

      string fname = basefilename + ".ose";
      std::ofstream outFile;
      outFile.open(fname.c_str(), std::ios::out);
      std::list<int>::iterator i = obstacle_sequence.begin();
      for (const int &i : obstacle_sequence) {
        outFile << i << " ";
      }
      outFile.close();
      int final_cost = _ct->get_optimal_cut_sequence(first_obstacle)->get_final_cost();
      std::cout << "final mu"
                << final_cost
                << std::endl;

      fname = basefilename + ".stats";
      std::ofstream statFile;
      statFile.open(fname.c_str(), std::ios::out);
      statFile << n_polies;
      statFile << ", " << _ct->n_fully_skipped;
      statFile << ", " << _ct->n_skipped;
      statFile << ", " << _ct->n_not_skipped;
      statFile << ", " << final_cost;
      statFile << ", " << _ct->get_optimal_cost();
      

      double proper_average;
      std::vector<int> n_choicesets_with_n_cutsequences;
      double avg = _ct->average_n_cutsequences_stats(
          proper_average, n_choicesets_with_n_cutsequences);
  
      statFile << ", " << proper_average;
      statFile << ", " << avg;
      statFile << std::endl;
      for ( const int &i : n_choicesets_with_n_cutsequences ) {
        statFile << i << ", ";
      }
      statFile << std::endl;
      statFile << _ct->get_optimal_cut_sequence(first_obstacle)->get_final_cost();
      
      statFile.close();
    }
  }
  
  return 0;
}
