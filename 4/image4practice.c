#include<stdio.h>
#include<stdlib.h>

  //char infile[1024];
  //char outfile[1024];
  FILE *infp, *outfp;

  struct IMAGEDATA{
      char magic [8];
      int width;
      int height;
      int max;
      int image[1024][1024];
      int imageout[1024][1024];

  };

  struct IMAGEDATA images;


 /*ファイルの読み込み*/
  int readImage(char infile[1024] ){
      /*ファイル読み込み専で開く*/	  
      //printf("読み込みファイル指定");
      //scanf("%s", infile);
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
  /*int maxValue(int size1, int size2,int value[size1][size2]){
    int maxImage = value[0][0];
    for(int i = 0; i < images.height; i++){
      for(int j = 0; j < images.width; j++){
         if(value[i][j] > maxImage){
          maxImage = value[i][j];
         }
      }
    }
        return maxImage;
  }

  int minValue(int size1, int size2, int value[size1][size2]){
    int minImage = value[0][0];
    for(int i = 0; i < size1; i++){
      for(int j = 0; j < size2; j++){
         if(minImage > value[i][j]){
          minImage = value[i][j];
         }
      }
    }
        return minImage;
  }*/

 

  int changeImage(){

    //int a = minValue(images.height,images.width, images.image);
    //int b = maxValue(images.height,images.width, images.image);
    //printf("a =%d\n",a);
    //printf("b =%d\n",b);
    /*//初期化
    for(int i = 0; i < images.height; i++){
      for(int j = 0; j < images.width; j++){
        images.image[i][j] = 0;
      }
    }
    */
    //濃度調節
    /*for(int i = 0; i < images.height; i++){
      for(int j = 0; j < images.width; j++){
        images.image[i][j] = a * images.image[i][j] + b;
      }
    }*/

    //コントラスト
    /*for(int i = 0; i < images.height; i++){
      for(int j = 0; j < images.width; j++){
        images.image[i][j] = images.max * (images.image[i][j] -a) / (b - a);
      }
    }*/
    

    //反転
    /*for(int i = 0; i < images.height; i++){
      for(int j = 0; j < images.width; j++){
        images.image[i][j] = images.max - images.image[i][j];
      } 
    }*/

    //移動平均法
    for(int i = 1; i < images.height; i++){
      for(int j = 1; j < images.width; j++){
        images.imageout[i][j] = (images.image[i-1][j-1]+ images.image[i-1][j]+ images.image[i-1][j+1]
                                 + images.image[i][j-1]+ images.image[i][j]+ images.image[i][j+1]
                                 + images.image[i+1][j-1]+ images.image[i+1][j]+ images.image[i+1][j+1]) /9;
      }
    }

    
  }

 /*ファイルの書き込み*/ 
  int writeImage(char outfile[1024]){
      /*指定のファイル書き込み専で開く*/	  
      //printf("書き込みファイルを指定");
      //scanf("%s", outfile);
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

      /*for(int i = 0; i < images.height; i++ ){
            for(int j = 0; j < images.width; j++){

              if(j == 9){
              fprintf(outfp, "% d\n", images.image[i][j]);
              }
              else{
               fprintf(outfp,"% d", images.image[i][j]);
              }

            }
      }*/

      for(int i = 0; i < images.height; i++ ){
            for(int j = 0; j < images.width; j++){

              if(j == 9){
              fprintf(outfp, "% d\n", images.imageout[i][j]);
              }
              else{
               fprintf(outfp,"% d", images.imageout[i][j]);
              }

            }
      }
      fclose(outfp);

      return 0;
  }

/*メイン関数*/
int main(int argc, char **argv){

  readImage(argv[1]);
  changeImage();
  writeImage(argv[2]);

}
