import roadrunner.*;
// import roadrunner.RoadRunner;
// import org.la4j.Matrices;
import org.la4j.Matrix;

public class roadrunner_java_sim_test1 {
  public static void main(String argv[]) {
    RoadRunner rr = new RoadRunner("/home/poltergeist/devel/models/decayModel.xml");
    System.out.println(rr.getExtendedVersionInfo());
    System.out.println("- Loaded rr");
    Matrix x = rr.simulate();
    System.out.println("- Simulate called");
    System.out.println(x);
  }
}
