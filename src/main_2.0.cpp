// tiled matrix multiplication
// cpu_matrix_mult_v2
// slicing mat_a_vec and mat_b_vec
// random generating matrix elements

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>


// transpose unit
void mat_transpose(std::vector<float> &mat_b_vec, std::vector<float> &mat_b_transpose_vec, int row_b, int col_b)
{
    int k = 0;

    for(int j = 0; j < col_b; j++){
        for(int i = 0; i < row_b; i++){
            mat_b_transpose_vec[k] = mat_b_vec[(i * col_b) + j];
            k++;
        }
    }

}

// matrix mul baseline
// mat_a(row_a x col_a)  x  mat_b(row_b x col_b)  =  mat_res_3(row_a x col_b)
void cpu_matrix_mult_v1(std::vector<float> &mat_a_vec, std::vector<float> &mat_b_vec, std::vector<float> &mat_res_1_vec, int row_a, int col_a, int col_b)
{
    for (int i = 0; i < row_a; ++i) 
    {
        for (int j = 0; j < col_b; ++j) 
        {
            float tmp = 0.0f;
            for (int k = 0; k < col_a; ++k)
            {
                tmp += mat_a_vec.data()[(i * col_a) + k] * mat_b_vec.data()[(k * col_b) + j];
            }
            mat_res_1_vec.data()[(i * col_b) +j] = tmp;
        }
    }
}


// matrix multiplication unit
void cpu_matrix_mult_v2(std::vector<float> &mat_a_slice_vec, std::vector<float> &mat_b_slice_vec, std::vector<float> &mat_res_slice_vec, int row_a, int col_a, int col_b)
{   
    

    for (int i = 0; i < row_a; ++i) 
    {
        for (int k = 0; k < col_b; ++k) 
        {
            float tmp = 0.0f;
            for (int j = 0; j < col_a; ++j)
            {
                tmp += mat_a_slice_vec.data()[(i * col_a) + j] * mat_b_slice_vec.data()[(k * col_a) + j];
            }
            mat_res_slice_vec.data()[(i * col_b) + k] = tmp;
            
        }
    }
}



int main()
{

    int row_a;
    int col_a;
    int row_b;
    int col_b;

    int tiled_matrix_memory_size;
    int num_of_elements_per_slice;
    int num_of_elements_in_last_iter;



    // =====Matries Dimensions input from user======
    // 1st matrix row and col size input
    std::cout<<"Enter number of row in array 1::";
    std::cin>>row_a;
    std::cout<<"Enter number of col in array 1::";
    std::cin>>col_a;

    // 2nd matrix row and col size input
    std::cout<<"Enter number of row in array 2::";
    std::cin>>row_b;
    std::cout<<"Enter number of col in array 2::";
    std::cin>>col_b;




    // =====Matries Dimensions======================
    // dimension of matrix a b and res
    int mat_a_dim = row_a * row_b;
    int mat_b_dim = row_b *col_b;
    int mat_res_dim = row_a * col_b;



    
    // =====Matries Declaration======================
    // matrix declaration as vector
    std::vector<float> mat_a_vec(mat_a_dim, 0);
    std::vector<float> mat_b_vec(mat_b_dim, 0);
    std::vector<float> mat_b_transpose_vec(mat_b_dim, 0);

    // result matrix declaration as vector
    std::vector<float> mat_res_1_vec(mat_res_dim);
    std::vector<float> mat_res_2_vec(mat_res_dim);
    



    // ==========take size of tile matrix memory from user========
    std::cout << "Enter size of tile matrix memory in BYTE:: ";
    std::cin >> tiled_matrix_memory_size;

    
    

    // =====Taking inputs for mat_a_vec and matr_b_vec======
    // take input for mat_a_vec
    std::cout << "Enter elements of 1st matrix:: " << std::endl;
    for(int i = 0; i < row_a * col_a; i++)
        std::cin >> mat_a_vec[i];
    
    // take input for mat_b_vec
    std::cout << "Enter elements of 2nd matrix:: " << std::endl;
    for(int i = 0; i < row_b * col_b; i++)
        std::cin >> mat_b_vec[i];
    



    // =====Transpose mat_b_vec and store it to mat_b_transpose_vec======
    mat_transpose(mat_b_vec, mat_b_transpose_vec, row_b, col_b);




    // =======Calculations for slicing 1st matrix mat_a_vec===============
    // number of elements we will take per tile
    num_of_elements_per_slice = tiled_matrix_memory_size / sizeof(float);

    // if number of elements per tile end up at the middle of the column of the 1st matrix
    if(num_of_elements_per_slice % col_a != 0){
        while(num_of_elements_per_slice % col_a != 0){
            num_of_elements_per_slice--;
        }
    }

    // number of elements in last iteration
    num_of_elements_in_last_iter = mat_a_vec.size() % num_of_elements_per_slice;


    // if number of elements in last iteration is smaller than number of elements in per tile, then we will add extra zeros to avoid segmentation fault
    if(num_of_elements_in_last_iter != 0){
        for(int i = 0; i < (num_of_elements_per_slice - num_of_elements_in_last_iter); i++)
            mat_a_vec.push_back(0);
    }




    // ==========New number of row of mat_a_slice_vec=======================  
    int mat_a_slice_row = num_of_elements_per_slice / col_a;
    


    // =====Matries slice Dimensions======================
    // dimension of matrix a_slice and res_slice
    int mat_a_slice_dim = num_of_elements_per_slice;
    int mat_b_slice_dim = row_b;  // Taking one colmun at a time as slice. There is a catch: For giant b matrix it might happen we can't take one single whole column.
    int mat_res_slice_dim = mat_a_slice_row * 1;




    // ===========declaration of mat_a_slice_vec for take slice of mat_a_vec 
    std::vector<float> mat_a_slice_vec(mat_a_slice_dim);
    
    // ===========declaration of mat_a_slice_vec for take slice of mat_a_vec 
    std::vector<float> mat_b_slice_vec(mat_b_slice_dim);
    
    // ===========declaration of mat_res_slice_vec for take slice of results
    std::vector<float> mat_res_slice_vec(mat_res_slice_dim);




    
    // ===========Inserting slices of mat_a_vec in multiplictaion module===============
    for(int i = 0; i < total_iterations_for_all_slice_chunk; i++){  // need to work on this
        
        // inserting slice of mat_a_vec to mat_a_slice_vec
        for(int j = 0; j < num_of_elements_per_slice; j++){
            mat_a_slice_vec.data()[j] = mat_a_vec.data()[(i * num_of_elements_per_slice) + j];
        }
        
        // inserting slice of mat_b_vec to mat_b_slice_vec
        for(int j = 0; j < col_b; j++){  // need to work on this
    
            for(int k = 0; k < row_b; k++){
                mat_b_slice_vec.data()[k] = mat_b_transpose_vec.data()[(j * row_b) + k];
            }

            // matrix multiplication module call
            cpu_matrix_mult_v2(mat_a_slice_vec, mat_b_slice_vec, mat_res_slice_vec, mat_a_slice_row, col_a, 1);

            // ===========Printing result matrix================================
            for(int l = 0; l < mat_res_slice_dim; l++){
                std::cout << mat_res_slice_vec.data()[l] << " ";
            }

            // inserting slice of result from mat_res_slice_vec to main result matrix mat_res_vec
            
                
            
            

            
        }

        
    }   


    cpu_matrix_mult_v1(mat_a_vec, mat_b_vec, mat_res_1_vec, row_a, col_a, col_b);


    //============Verification=========================


    for(int i = 0; i < mat_res_dim; i++){
        if(mat_res_1_vec.data()[i] != mat_res_2_vec.data()[i])
            std::cout << "Mismatched!!!";
    }




















    // ===========Printing result matrix================================
    // for(int i = 0; i < mat_res_dim; i++){
    //     std::cout << mat_res_vec.data()[i] << " ";
    // }





    return 0;
}
