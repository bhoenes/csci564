package lab0;

/**
 * Benchmark recursive smoothing filter in Java.
 * @author Dave Hale, Colorado School of Mines
 * @version 2011.08.19
 */

import java.util.Random;
import java.util.Arrays;

public class Lab0 {
  public static void main(String[] args) {
    System.out.println("Java");
    int n = 1500;
    int n2 = n*n;
    int n3 = n2*n;
    double[][] A = new double[n][n];
    double[][] B = new double[n][n];
    double[][] C = new double[n][n];

		Random rng = new Random();

		for(int i1=0; i1 < n; i1++) {
			for(int i2=0; i2 < n; i2++) {
				A[i2][i1] = rng.nextDouble();
				B[i2][i1] = rng.nextDouble();
			}
		}
		
    Stopwatch sw = new Stopwatch();
    sw.start();
    BchAtlas.step00(A, B, C);
    sw.stop();
    System.out.println("step00 mflops= "+mflops(2,n,sw.time()));
    
		for(int t = 100; t<141; t++) {
	  	Lab0.zeroFill(C);
  		sw.restart();
  		BchAtlas.step01(t, A, B, C);
  		sw.stop();
  		System.out.println("step01 t= "+t+" mflops= "+mflops(2,n,sw.time()));
  	}

    
    
  }
  
  public static void zeroFill(double[][] array) {
    	for (double[] row : array)
		    Arrays.fill(row, 0);
	}
  
  public static int mflops(final int flops_per_iteration, final int n, final double elapsed_time) {
		return (int)((0.000001*n*n*n*flops_per_iteration)/elapsed_time);
	}
}
