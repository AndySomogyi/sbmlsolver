import roadrunner.*;
// import roadrunner.RoadRunner;

public class roadrunner_javajni_test1 {
  public static void main(String argv[]) {
    RoadRunner rr = new RoadRunner();
    System.out.println(rr.getExtendedVersionInfo());
    System.out.println("- Loaded rr");
  }
}
