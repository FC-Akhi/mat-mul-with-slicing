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
void cpu_matrix_mult_v2(std::vector<float> &mat_a_vec, std::vector<float> &mat_b_transpose_vec, std::vector<float> &mat_res_2_vec, int row_a, int col_a, int col_b)
{   
    
    

    for (int i = 0; i < row_a; ++i) 
    {
        for (int k = 0; k < col_b; ++k) 
        {
            float tmp = 0.0;
            for (int j = 0; j < col_a; ++j)
            {
                tmp += mat_a_vec.data()[(i * col_a) + j] * mat_b_transpose_vec.data()[(k * col_a) + j];
            }
            mat_res_2_vec.data()[(i * col_b) + k] = tmp;
            // m++;
        }
    }
}




    // matrix multiplication unit
void cpu_matrix_mult_v3(std::vector<float> &mat_a_slice_vec, std::vector<float> &mat_b_slice_vec, std::vector<float> &mat_res_slice_vec, int row_a, int col_a, int col_b)
{   
    
    for (int i = 0; i < row_a; ++i) 
    {
        for (int k = 0; k < col_b; ++k) 
        {
            float tmp = 0.0;
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
    int mat_a_dim = row_a * col_a;
    int mat_b_dim = row_b *col_b;
    int mat_res_dim = row_a * col_b;



    
    // =====Matries Declaration======================
    // matrix declaration as vector
    std::vector<float> mat_a_vec(mat_a_dim, 0.0);
    std::vector<float> mat_b_vec(mat_b_dim, 0.0);
    std::vector<float> mat_b_transpose_vec(mat_b_dim, 0.0);

    // result matrix declaration as vector
    std::vector<float> mat_res_2_vec(mat_res_dim);
    std::vector<float> mat_res_3_vec(mat_res_dim);

    //==========Variable for Bram size==============
    int tiled_matrix_memory_size;

    // ==========take size of tile matrix memory from user========
    std::cout << "Enter size of tile matrix memory in BYTE:: ";
    std::cin >> tiled_matrix_memory_size;




    // =====Taking inputs for mat_a_vec and matr_b_vec======
    // generating int values for mat_a_vec 0 to 2
    for(int i = 0; i < mat_a_dim; i++)
        mat_a_vec.data()[i] = float((rand() % 100) + 0);  //Generate number between 0 to 2
    

    // generating int values for mat_b_vec 0 to 2
    for(int i = 0; i < mat_b_dim; i++)
        mat_b_vec.data()[i] = float((rand() % 100) + 0);  //Generate number between 0 to 2

    for(int i = 0; i < mat_a_dim; i++)
        std::cout << "Matrix 1: " << mat_a_vec.data()[i] << " ";
    std::cout<<"\n";

    for(int i = 0; i < mat_b_dim; i++)
        std::cout << "Matrix 2: " << mat_b_vec.data()[i] << " ";
    std::cout<<"\n";


    // =====Transpose mat_b_vec and store it to mat_b_transpose_vec======
    mat_transpose(mat_b_vec, mat_b_transpose_vec, row_b, col_b);
    cpu_matrix_mult_v2(mat_a_vec, mat_b_transpose_vec, mat_res_2_vec, row_a, col_a, col_b);

    



    //================Necessary variables for slicing====================
    int num_of_elements_per_slice_mat_a;
    int num_of_elements_per_slice_mat_b;

    int num_of_elements_in_last_iter_mat_a;
    int num_of_elements_in_last_iter_mat_b;

    int num_of_iters_for_mat_a;
    int num_of_iters_for_mat_b;
    
    int num_of_rows_mat_a_slice;
    int num_of_cols_mat_b_slice;




    // =======Calculations for slicing 1st & 2nd matrix===============
    // number of elements we will take per tile
    num_of_elements_per_slice_mat_a = tiled_matrix_memory_size / sizeof(float);
    num_of_elements_per_slice_mat_b = tiled_matrix_memory_size / sizeof(float);

    // if number of elements per tile end up at the middle of the column of the 1st matrix
    if(num_of_elements_per_slice_mat_a % col_a != 0){
        while(num_of_elements_per_slice_mat_a % col_a != 0){
            num_of_elements_per_slice_mat_a--;
        }
    }
    num_of_elements_per_slice_mat_b = num_of_elements_per_slice_mat_a;



    // number of elements in last iteration
    num_of_elements_in_last_iter_mat_a = mat_a_vec.size() % num_of_elements_per_slice_mat_a;

    num_of_elements_in_last_iter_mat_b = mat_b_vec.size() % num_of_elements_per_slice_mat_b;




    // number of iterations for mat_a_vec
    num_of_iters_for_mat_a = ceil(float(mat_a_vec.size()) / float(num_of_elements_per_slice_mat_a));

    // number of iterations for mat_b_vec
    num_of_iters_for_mat_b = ceil(float(mat_b_vec.size()) / float(num_of_elements_per_slice_mat_b));

    std::cout<< "num_of_iters_for_mat_a " << num_of_iters_for_mat_a << std::endl;
    std::cout<< "num_of_iters_for_mat_b " << num_of_iters_for_mat_b << std::endl;


    // number of rows for mat_a_slice_vec
    num_of_rows_mat_a_slice = num_of_elements_per_slice_mat_a / col_a;

    // number of cols for mat_b_slice_vec
    num_of_cols_mat_b_slice = num_of_elements_per_slice_mat_b / row_b;





    // =====Matries slice Dimensions======================
    // dimension of matrix a_slice and res_slice
    int mat_a_slice_dim = num_of_rows_mat_a_slice * col_a;
    int mat_b_slice_dim = row_b * num_of_cols_mat_b_slice;  // Taking one colmun at a time as slice. There is a catch: For giant b matrix it might happen we can't take one single whole column.
    int mat_res_slice_dim = num_of_rows_mat_a_slice * num_of_cols_mat_b_slice;




    // ===========declaration of mat_a_slice_vec for take slice of mat_a_vec 
    std::vector<float> mat_a_slice_vec(mat_a_slice_dim);
    
    // ===========declaration of mat_a_slice_vec for take slice of mat_a_vec 
    std::vector<float> mat_b_slice_vec(mat_b_slice_dim);
    
    // ===========declaration of mat_res_slice_vec for take slice of results
    std::vector<float> mat_res_slice_vec(mat_res_slice_dim);



    

    int tmp_1 = num_of_elements_per_slice_mat_a;
    int tmp_2 = num_of_cols_mat_b_slice;
    

    // ===========Inserting slices of mat_a_vec in multiplictaion module===============
    for(int i = 0; i < num_of_iters_for_mat_a; i++){  // need to work on this
        
        if(i == (num_of_iters_for_mat_a - 1) && num_of_elements_in_last_iter_mat_a != 0){

                tmp_1 = num_of_elements_per_slice_mat_a;
                
                num_of_elements_per_slice_mat_a = num_of_elements_in_last_iter_mat_a;
            
                num_of_rows_mat_a_slice =  num_of_elements_per_slice_mat_a / col_a;
                
                mat_res_slice_dim = num_of_rows_mat_a_slice * num_of_cols_mat_b_slice;
                

        }

        // inserting slice of mat_a_vec to mat_a_slice_vec
        for(int j = 0; j < num_of_elements_per_slice_mat_a; j++){
            mat_a_slice_vec.data()[j] = mat_a_vec.data()[(i * tmp_1) + j];
        }
        for(int i = 0; i < num_of_elements_per_slice_mat_a; i++)
                std::cout << "Matrix a slice: " << i << " "  << mat_a_slice_vec.data()[i] << std::endl;
        std::cout<<"\n";
        

        // inserting slice of mat_b_vec to mat_b_slice_vec
        for(int j = 0; j < num_of_iters_for_mat_b; j++){  // need to work on this

            if(j == (num_of_iters_for_mat_b - 1) && num_of_elements_in_last_iter_mat_b != 0){

                
                num_of_elements_per_slice_mat_b = num_of_elements_in_last_iter_mat_b;
            
                num_of_cols_mat_b_slice =  num_of_elements_per_slice_mat_b / row_b;
                
                mat_res_slice_dim = num_of_rows_mat_a_slice * num_of_cols_mat_b_slice;

            }

            for(int k = 0; k < num_of_elements_per_slice_mat_b; k++){
                mat_b_slice_vec.data()[k] = mat_b_transpose_vec.data()[(j * tmp_1) + k];
            }

            for(int i = 0; i < num_of_elements_per_slice_mat_b; i++)
                std::cout << "Matrix b slice: " << i << " "  << mat_b_slice_vec.data()[i] << std::endl;
            std::cout<<"\n";

            // matrix multiplication module call
            cpu_matrix_mult_v3(mat_a_slice_vec, mat_b_slice_vec, mat_res_slice_vec, num_of_rows_mat_a_slice, col_a, num_of_cols_mat_b_slice);
            
            for(int i = 0; i < mat_res_slice_dim; i++)
                std::cout << "Matrix res slice: " << i << " "  << mat_res_slice_vec.data()[i] << std::endl;
            std::cout<<"\n";
            

            // inserting slice of result from mat_res_slice_vec to main result matrix mat_res_vec
            int m = 0;
            for(int s = i * num_of_rows_mat_a_slice; s < ((i + 1) * num_of_rows_mat_a_slice); s++){
                
                if(j == (num_of_iters_for_mat_b - 1) && num_of_elements_in_last_iter_mat_b != 0)
                    for(int t = j * tmp_2 ; t < col_b; t++){
                        mat_res_3_vec.data()[(s * col_b) + t] = mat_res_slice_vec.data()[m];
                        m++;
                    }
                else{
                    for(int t = j * tmp_2 ; t < ((j + 1) * num_of_cols_mat_b_slice); t++ ){
                        mat_res_3_vec.data()[(s * col_b) + t] = mat_res_slice_vec.data()[m];
                        m++;
                    }
                }
            

            }
            
            
        }
        num_of_elements_per_slice_mat_b = tmp_1;
        num_of_cols_mat_b_slice =  num_of_elements_per_slice_mat_b / row_b;
        
    }




    for(int i = 0; i < mat_res_dim; i++){
        if(mat_res_2_vec.data()[i] != mat_res_3_vec.data()[i])
            std::cout << i << " " << mat_res_2_vec[i] << " " << mat_res_3_vec[i] << std::endl;
    }
    std::cout << "\n";


    return 0;
}
