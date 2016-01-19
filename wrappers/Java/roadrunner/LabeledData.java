package roadrunner;

import org.la4j.Matrices;
import org.la4j.matrix.MatrixFactory;
import org.la4j.Matrix;
import org.la4j.matrix.DenseMatrix;

import roadrunner.*;

public class LabeledData {
  private Matrix m;

  public LabeledData(double[][] data) {
    m = DenseMatrix.from2DArray(data);
  }

  public String toString() {
    return m.toString();
  }
}
