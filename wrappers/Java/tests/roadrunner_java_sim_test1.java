import roadrunner.*;
// import roadrunner.RoadRunner;
// import org.la4j.Matrices;

public class roadrunner_java_sim_test1 {
  public static void main(String argv[]) {
    RoadRunner rr = new RoadRunner("/home/poltergeist/devel/models/decayModel.xml");
    System.out.println(rr.getExtendedVersionInfo());
    System.out.println("Loaded rr");
    rr.simulate();
  }
}
