#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void print_matrix ( glm::mat4 * mat ) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      cout << "mat[" << i << "][" << j << "] = " << (*mat)[i][j] << "\t";
    }
    cout << endl;
  }
}

int main () {
  glm::mat4 mat = glm::mat4(1.0f);
  print_matrix ( &mat );
}
  
