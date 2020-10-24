#include<stdio.h>
#include<stdlib.h>

  FILE *infp, *outfp;

  struct IMAGEDATA{
      char magic [8];
      int width;
      int height;
      int max;
      int image[1024][1024];
      int a[9];
      int imageout[1024][1024];
      int bsize;

  };

  struct IMAGEDATA images;


 /*ファイルの読み込み*/
  int readImage(char infile[1024] ){
      /*ファイル読み込み専で開く*/	  

      infp = fopen(infile, "r");
      if(infp == NULL){
        printf("file open error");
        exit(EXIT_FAILURE);
      }
      /*ファイルからのデータ読み込み*/
      fscanf(infp, "%s", images.magic);  
      fscanf(infp, "%d" ,&images.width);
      fscanf(infp, "%d" ,&images.height);
      fscanf(infp, "%d" ,&images.max);


      for(int i = 0; i <images.height; i ++){
          for(int j = 0; j <images.width; j++){
            fscanf(infp, "%d", &images.image[i][j]);          
	  }
      }
     

      fclose(infp);

      return 0;
  }

  

 

  int changeImage(int size){
    images.bsize = size;


    for(int i = 0; i<= images.height-images.bsize; i+=images.bsize){
      for(int j = 0; j <=images.width-images.bsize; j+= images.bsize){

        //平均値を求める
        int total = 0;

        for(int k = 0; k < images.bsize; k++){
          for(int l = 0; l <  images.bsize; l++){

            
            total += images.image[i+k][j+l];
           

        //平均値の代入
        for(int k = 0; k < images.bsize; k++){
          for(int l = 0; l <  images.bsize; l++){
            images.image[i+k][j+l] =  total /(images.bsize * images.bsize);
            
          }
        }

      }
    }


    return 0;
    
  }

 /*ファイルの書き込み*/ 
  int writeImage(char outfile[1024]){
      /*指定のファイル書き込み専で開く*/	  

      outfp = fopen(outfile, "w");

      if(outfp == NULL){
        printf("file open error");
        exit(EXIT_FAILURE);
      }

      /*ファイルへのデータ書き出し*/
      fprintf(outfp,"%s\n", images.magic);
      fprintf(outfp, "%d", images.width);
      fprintf(outfp, "% d\n", images.height);
      fprintf(outfp, "%d\n", images.max);


      for(int i = 0; i < images.height; i++ ){
            for(int j = 0; j < images.width; j++){

              if(j == 9){
              fprintf(outfp, "% d\n", images.image[i][j]);
              }
              else{
               fprintf(outfp,"% d", images.image[i][j]);
              }

            }
      }
      fclose(outfp);

      return 0;
  }

/*メイン関数*/
int main(int argc, char **argv){

  readImage(argv[1]);
 
  int size = atoi(argv[3]);
  changeImage(size);
  writeImage(argv[2]);

}
