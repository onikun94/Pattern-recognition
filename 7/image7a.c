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

  void bubble_sort(int a[], int n){
    int t = 0;
    for(int i = 0; i< n-1; i++){
      for(int j = n -1;  j > i; j-- ){
        if(a[j-1] > a[j]){
          t = a[j];
          a[j] = a[j-1];
          a[j-1] = t;
        }
      }
    }
  }

 

  int changeImage(int threshold){

    //メディアンフィルタ法(ノイズ除去に用いる)
    for(int i = 1; i< images.height -1; i++){
      for(int j = 1; j < images.width -1; j++){
        images.a[0] = images.image[i-1][j-1];
        images.a[1] = images.image[i-1][j];
        images.a[2] = images.image[i-1][j+1];
        images.a[3] = images.image[i][j-1];
        images.a[4] = images.image[i][j];
        images.a[5] = images.image[i][j+1];
        images.a[6] = images.image[i+1][j-1];
        images.a[7] = images.image[i+1][j];
        images.a[8] = images.image[i+1][j+1];

        //ソート関数
        bubble_sort(images.a, 9);

        images.imageout[i][j] = images.a[4];//メディアンフィルタ適用後の画像を用いる
      }
    }
    
    //二値化処理
    for(int i = 0; i < images.height; i++){
        for(int j = 0; j < images.width; j++){
            if(images.imageout[i][j] > threshold){
                images.imageout[i][j] = images.max;
            }
            else{
                images.imageout[i][j] = 0;
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
  int threshold = atoi(argv[3]);
  changeImage(threshold);
  writeImage(argv[2]);

}
