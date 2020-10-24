#include<stdio.h>
#include<stdlib.h>

  FILE *infp, *outfp;

  struct IMAGEDATA{
      char magic [8];
      int width;
      int height;
      int max;
      int image[1024][1024];
      int dataLabel;

  };

  struct IMAGEDATA images;

  struct LABELDATA{
      int area;
      int totalX;
      int totalY;
      int barycenterX;
      int barycenterY;
  };

  struct LABELDATA labels[8];

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

  //ラベル付関数
  int addLabel(int i, int j, int label){
      images.image[i][j] = label;
      if(images.image[i-1][j-1] ==images.max){
          addLabel(i-1,j-1,label);
      }
      if(images.image[i-1][j] ==images.max){
          addLabel(i-1,j,label);
      }
      if(images.image[i-1][j+1] ==images.max){
          addLabel(i-1,j+1,label);
      }
      if(images.image[i][j-1] ==images.max){
          addLabel(i,j-1,label);
      }
      if(images.image[i][j+1] ==images.max){
          addLabel(i,j+1,label);
      }
      if(images.image[i+1][j-1] ==images.max){
          addLabel(i+1,j-1,label);
      }
      if(images.image[i+1][j] ==images.max){
          addLabel(i+1,j,label);
      }
      if(images.image[i+1][j+1] ==images.max){
          addLabel(i+1,j+1,label);
      }
  } 

  int labelImage();

  int changeImage(){

    //ラベリング
    int label = 1;
    for(int i = 0; i< images.height; i++){
        for(int j = 0; j < images.width; j++){
            if(images.image[i][j] == images.max){
                addLabel(i, j, label);
                label ++;
            }
            
        }
    }
    images.dataLabel = label;

    
    labelImage();
  
  }

  int labelImage(){
      //初期化 
      for(int i = 0; i < images.dataLabel; i++ ){
          labels[i].area = 0;
          labels[i].totalX = 0;
          labels[i].totalY = 0;
          labels[i].barycenterX = 0;
          labels[i].barycenterY = 0;
      }

      for(int y = 0; y < images.height; y ++){
          for(int x = 0; x < images.width; x ++){
              labels[images.image[y][x]].area ++;
              labels[images.image[y][x]].totalX += x;
              labels[images.image[y][x]].totalY += y;
          }
      }

      for(int i = 0; i < images.dataLabel; i++ ){
          labels[i].barycenterX = labels[i].totalX /labels[i].area;
          labels[i].barycenterY = labels[i].totalY /labels[i].area;
      }

      for(int i = 0; i< images.dataLabel; i++){
          printf("labelNumber = %d,  area = %d, xwt = %d, ywt = %d \n",i, labels[i].area, labels[i].barycenterX, labels[i].barycenterY);
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
  changeImage();
  writeImage(argv[2]);

}
