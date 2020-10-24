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
  int maxValue(){
    int maxImage = 0;
    for(int i = 0; i < images.height; i++){
      for(int j = 0; j < images.width; j++){
         if(images.image[i][j] > maxImage){
          maxImage = images.image[i][j];
         }
      }
    }
        return maxImage;
  }

  int minValue(){
    int minImage = 0;
    for(int i = 0; i < images.height; i++){
      for(int j = 0; j < images.width; j++){
         if(minImage > images.image[i][j]){
          minImage = images.image[i][j];
         }
      }
    }
        return minImage;
  }

 

  int changeImage(){

    int a = minValue();
    int b = maxValue();
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
    for(int i = 0; i < images.height; i++){
      for(int j = 0; j < images.width; j++){
        images.image[i][j] = images.max * (images.image[i][j] -a) / (b - a);
      }
    }
    

    //反転
    /*for(int i = 0; i < images.height; i++){
      for(int j = 0; j < images.width; j++){
        images.image[i][j] = images.max - images.image[i][j];
      } 
    }*/


    
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

      for(int i = 0; i < images.height; i++ ){
            for(int j = 0; j < images.width; j++){
              /*if(j == 0){
              fprintf(outfp, "%d", images.image[i][j]);
              }*/
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
  changeImage();
  writeImage(argv[2]);

}
