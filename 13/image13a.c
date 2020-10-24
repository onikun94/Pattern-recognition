#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

  FILE *infp, *tempfp, *outfp, *outtmpfp;

  struct IMAGEDATA{
      char magic [8];
      int width;
      int height;
      int max;
      int image[1024][1024];
      int imageout[1024][1024];

  };

  struct IMAGEDATA images;


  struct TEMPLATEDATA{
      char mg[8];
      int w;
      int h;
      int mx;
      int T[1024][1024];
      int suitX;
      int suitY;
  };

  struct TEMPLATEDATA temp;
 

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

  int readtempImage(char infile[1024] ){
      /*ファイル読み込み専で開く*/	  

      tempfp = fopen(infile, "r");
      if(tempfp == NULL){
        printf("file open error");
        exit(EXIT_FAILURE);
      }
      /*ファイルからのデータ読み込み*/
      fscanf(tempfp, "%s", temp.mg);  
      fscanf(tempfp, "%d" ,&temp.w);
      fscanf(tempfp, "%d" ,&temp.h);
      fscanf(tempfp, "%d" ,&temp.mx);


      for(int i = 0; i <temp.h; i ++){
          for(int j = 0; j <temp.w; j++){
            fscanf(tempfp, "%d", &temp.T[i][j]);          
	  }
      }
     

      fclose(infp);

      return 0;
  }

  

    
  
  //すべての画像処理に関する関数
  int changeImage(){
      int minD = INT_MAX;
      for (int y = 0; y <= images.height - temp.h; y++)
      {
          for (int x = 0; x <= images.width - temp.w; x++)
          {
              int D = 0;
              for (int m = 0; m < temp.h; m++)
              {
                  for (int n = 0; n < temp.w; n++)
                  {
                      if (D > minD)
                      {
                          break;
                      }
                      D += abs(images.image[y + m][x + n] - temp.T[m][n]);
                  }
              }

              if (D < minD)
              {
                  minD = D;
                  temp.suitX = x;
                  temp.suitY = y;
              }
          }
      }
      printf("tempx = %d tempy =%d\n ", temp.suitX,temp.suitY);
      printf("minD = %d\n",minD);

      //新たなテンプレート抽出
      for(int i = 0; i < temp.h; i++){
        for(int j = 0; j < temp.w; j++){
          images.imageout[i][j] = images.image[temp.suitY  +i][temp.suitX  +j];
        }
      }

      //白線描画
      for(int x = temp.suitX; x < temp.suitX+ temp.w; x++){
          images.image[temp.suitY][x] = images.max;
          images.image[temp.suitY+temp.h][x] = images.max;
      }

      for(int y = temp.suitY; y <temp.suitY+temp.h; y++){
          images.image[y][temp.suitX] = images.max;
          images.image[y][temp.suitX+temp.w] = images.max;
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

   int writeTempImage(char outfile[1024]){
      /*指定のファイル書き込み専で開く*/	  

      outtmpfp = fopen(outfile, "w");

      if(outtmpfp == NULL){
        printf("file open error");
        exit(EXIT_FAILURE);
      }

      /*ファイルへのデータ書き出し*/
      fprintf(outtmpfp,"%s\n", images.magic);
      fprintf(outtmpfp, "%d", temp.w);
      fprintf(outtmpfp, "% d\n", temp.h);
      fprintf(outtmpfp, "%d\n", images.max);

      for(int i = 0; i < temp.h; i++ ){
            for(int j = 0; j < temp.w; j++){

              if(j == 9){
              fprintf(outtmpfp, "% d\n", images.imageout[i][j]);
              }
              else{
               fprintf(outtmpfp,"% d", images.imageout[i][j]);
              }
             

            }
      }
      fclose(outtmpfp);

      return 0;
  }

/*メイン関数*/
int main(int argc, char **argv){

  readImage(argv[1]);
  readtempImage(argv[2]);
  changeImage();
  writeImage(argv[3]);
  writeTempImage(argv[4]);

}
