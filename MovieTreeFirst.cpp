#include <string>
#include "MovieTree.hpp"
#include <iostream>
#include <fstream>
using namespace std;



void deleteTree(MovieNode *node){
  if(node == nullptr){
    return;
  }
  else{
    if(node->leftChild == nullptr){
      return;
    }
    else{
      deleteTree(node->leftChild);
    }
    if(node->rightChild == nullptr){
      return;
    }
    else{
      deleteTree(node->rightChild);
    }
  }
  delete node;
}

MovieTree::MovieTree(){
  root = nullptr;
}

MovieNode* searchHelper(std::string title, MovieNode *node){
  if(node == nullptr || node->title == title){
    return node;
  }
  if(title < node->title){
    return searchHelper(title, node->leftChild);
  }
  return(searchHelper(title, node->rightChild));
}

MovieNode* MovieTree::search(std::string title){
  return searchHelper(title, root);
}

MovieTree::~MovieTree(){
  deleteTree(root);
}

void printMovie(MovieNode *node){
  if (node == NULL){
    return;
  }
  printMovie(node->leftChild);
  std::cout << "Movie: " << node->title << " " << node->rating << std::endl;
  printMovie(node->rightChild);
}

void MovieTree::printMovieInventory(){
  printMovie(root);
}

void newMovie(MovieNode *node, int ranking, std::string title, int year, float rating){
  if(title < node->title){
    if(node->leftChild == NULL){
      MovieNode *nodeNew = new MovieNode(ranking, title, year, rating);
      node->leftChild = nodeNew;
      nodeNew->parent = node;
      return;
    }
    else{
      newMovie(node->leftChild, ranking, title, year, rating);
      return;
    }
  }
  else{
    if(node->rightChild == NULL){
      MovieNode *nodeNew = new MovieNode(ranking, title, year, rating);
      node->rightChild = nodeNew;
      nodeNew->parent = node;
      return;
    }
    else{
      newMovie(node->rightChild, ranking, title, year, rating);
    }
  }
}

void MovieTree::addMovieNode(int ranking, std::string title, int year, float rating){
  if(root == NULL){
    root = new MovieNode(ranking, title, year, rating);
    return;
  }
  newMovie(root, ranking, title, year, rating);
  return;
}

void MovieTree::findMovie(std::string title){
  MovieNode *node = search(title);
  if(node != nullptr){
    std::cout << "Movie Info:" << std::endl;
    std::cout << "==================" << std::endl;
    std::cout << "Ranking:" << node->ranking << std::endl;
    std::cout << "Title  :" << node->title << std::endl;
    std::cout << "Year   :" << node->year << std::endl;
    std::cout << "rating :" << node->rating << std::endl;
  }
  else{
    std::cout << "Movie not found." << std::endl;
  }
}

void queryMovieHelper(MovieNode *node, float rating, int year){
  if (node == NULL){
    return;
  }
  if(node->rating >= rating && node->year > year ){
    std::cout << node->title << "(" << node->year << ") " << node->rating << std::endl;
  }
  queryMovieHelper(node->leftChild, rating, year);
  queryMovieHelper(node->rightChild, rating, year);
}

void MovieTree::queryMovies(float rating, int year){
  std::cout << "Movies that came out after " << year << " with rating at least " << rating << ":" << std::endl;
  queryMovieHelper(root, rating, year);
}

float sumHelper(MovieNode *node){
  if (node == NULL){
    return 0;
  }
  float sum = 0;

  sum = sum + sumHelper(node->leftChild);
  sum = sum + sumHelper(node->rightChild);
  sum = sum + node->rating;

  return sum;
}

int countHelper(MovieNode *node){
  if (node == NULL){
    return 0;
  }
  int count = 0;
  count = count + 1;
  count = count + countHelper(node->leftChild);
  count = count + countHelper(node->rightChild);
  return count;
}

void MovieTree::averageRating(){
if(root == NULL){
    std::cout << "Average rating:" << "0" << std::endl;
    return;
}
  std::cout << "Average rating:" << sumHelper(root)/countHelper(root) << std::endl;
}



int main(int argc, char *argv[]){
  MovieTree tree;
  std::string title = "";
  std::string input = "0";
  std::string ranking = "";
  std::string year = "";
  std::string rating = "";
  ifstream movieStream(argv[1]);
  while(getline(movieStream, ranking, ',')){
    getline(movieStream, title, ',');
    getline(movieStream, year, ',');
    getline(movieStream, rating);
    tree.addMovieNode(stoi(ranking), title, stoi(year), stof(rating));
  }
  while(stoi(input) != 5){
    std::cout << "======Main Menu======" << std::endl;
    std::cout << "1. Find a movie" << std::endl;
    std::cout << "2. Query movies" << std::endl;
    std::cout << "3. Print the inventory" << std::endl;
    std::cout << "4. Average Rating of movies" << std::endl;
    std::cout << "5. Quit" << endl;
    getline(std::cin,input);
    switch(stoi(input)){
      case 1:
        std::cout << "Enter title:" << std::endl;
        getline(cin, title);
        tree.findMovie(title);
        break;
      case 2:
        std::cout << "Enter minimum rating:" << std::endl;
        getline(cin, rating);
        std::cout << "Enter minimum year:" << std::endl;
        getline(cin, year);
        tree.queryMovies(stof(rating), stoi(year));
        break;
      case 3:
        tree.printMovieInventory();
        break;
      case 4:
        tree.averageRating();
        break;
      case 5:
        std::cout << "Goodbye!" << std::endl;
        break;
    }
  }
}
