package lab0;

public class BchAtlas {
  public static void step00(final double[][] A, final double[][] B, double[][] C) {
    int n = A.length;
    int i, j, k;
    double R;
    for(i=0; i<n; i++) {
    	for(k=0; k<n; k++) {
    		R=B[i][k];
    		for(j=0; j<n; j++) {
	    		C[i][j] += R * A[k][j];
    		}
    	}
  	}
  }
  
  public static void step01(final double[][] A, final double[][] B, double[][] C) {
  	step01(12, A, B, C);
  }
  
  public static void step01(final int tileSize, final double[][] A, final double[][] B, double[][] C) {
  	int n = A.length;
  	int i, j, k, jj, kk;
  	double R;
  	for(kk= 0; kk<n; kk+=tileSize){
  		for(jj=0; jj<n; jj+=tileSize) {
  			for(i=0; i<n; i++) {
  				for(k=kk; k<Math.min(kk+tileSize, n); k++){
		    		R=B[i][k];
		    		for(j=jj; j<Math.min(jj+tileSize, n); j++) {
		    			C[i][j] += R * A[k][j];
		    		}
  				}
  			}
  		}
  	}
  }
  
  public static void step05(final double[][] A, final double[][] B, double[][] C) {
  	step05(12, A, B, C);
  }
  
  public static void step05(final int tileSize, final double[][] A, final double[][] B, double[][] C) {

}
