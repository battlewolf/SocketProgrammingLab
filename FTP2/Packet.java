import java.io.Serializable;
import java.util.Arrays;


public class Packet implements Serializable {
    Operations operation = null;
    String fileName = null;

    Packet(Operations operation) {
        this.operation = operation;

    }

    Packet (Operations operation, String fileName) {
        this.operation = operation;
        this.fileName = fileName;
    }
}

