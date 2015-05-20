import roadrunner.*;
// import roadrunner.RoadRunner;

public class roadrunner_java_sim_test1 {
  public static void main(String argv[]) {
    RoadRunner rr = new RoadRunner("/home/poltergeist/devel/models/GlycolysisOriginal.xml");
    System.out.println(rr.getExtendedVersionInfo());
    System.out.println("Loaded rr");
    rr.simulate();
  }
}
