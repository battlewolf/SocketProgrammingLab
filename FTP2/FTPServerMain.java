import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * MultiThreaded FTP Server Using ExecutorService
 */
/**
 * @author vinay-3264
 *
 */


class FTPServerThread implements Runnable {
	private Socket clientSocket;/*
	private BufferedReader in;
	private PrintWriter out;*/

	
	private ObjectInputStream commandIn;
	private Thread thread;
	private ObjectOutputStream out;
	
	
	public FTPServerThread(Socket clientSocket) {
		this.clientSocket = clientSocket;
		this.thread = Thread.currentThread();
	}
	
	
	private void initStreams() {
		try {
			commandIn =new ObjectInputStream(clientSocket.getInputStream());
			out = new ObjectOutputStream(clientSocket.getOutputStream());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	private void closeStreams() {
	
		try {
			commandIn.close();
			out.close();
			clientSocket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	
	
	public void run() {
		boolean exit = false;
		initStreams();
		while(!exit) {
			try {
				Packet recvOperation = null;
				try {
					recvOperation = (Packet ) commandIn.readObject();
					
				} catch (ClassNotFoundException e) {
					// TODO Auto-generated catch block
					System.out.println("error occurred during transmission");
					e.printStackTrace();
					continue;
				}
				System.out.print(clientSocket.getInetAddress().getHostAddress()+" wants ");

				switch (recvOperation.operation) {
				case EXIT: {
					exit = true;
					System.out.println("to exit");
					break;
				}
				case GET: {
					if (recvOperation != null) {
						System.out.print("the file "+recvOperation.fileName);
						String fileName = recvOperation.fileName;
						try {
							File sendFile = new File(fileName);
							if (!(sendFile.exists())) {
								out.writeObject(null);
							}
							else {
								out.writeObject(sendFile);
							}
						}
						catch (NullPointerException e) {
							System.out.println("which is an Invalid file -_-");
						}
						break;
					}
				}
				case LIST: {
					System.out.println("to list ");
					out.writeObject((new File(".")).listFiles());
					break;
				}
				
				}
				
			} catch (IOException e) {
				e.printStackTrace();
				break;
			} 
		} 
		System.out.println("Thread "+thread.getId()+" quits!!!");
	}
}



class FTPServer implements Runnable {
	private int port;
	private ServerSocket serverSocket = null;
	private boolean serverType = false; //0 - naive multithreading //1-threadpool based 
	private static final int NUM_THREADS = 100;
	
	FTPServer(int port, boolean serverType) {
		this.port = port;
		this.serverType = serverType;
	}
	
	void initSocket() {
		try {
			serverSocket = new ServerSocket(port);
			System.out.println("Server started !!!! Running on port "+port);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void run() {
		initSocket();
		ExecutorService pool = Executors.newFixedThreadPool(NUM_THREADS);
		while(true) {
			try {
				Socket clientSocket = serverSocket.accept();
				System.out.println("Client "+clientSocket.getInetAddress().getHostAddress()+" connected to server");
				if(!serverType) {
					(new Thread(new FTPServerThread(clientSocket))).start();
				}
				else {
					pool.execute(new FTPServerThread(clientSocket));
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
//		serverSocket.close();
	}
}



public class FTPServerMain {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		(new Thread(new FTPServer(10000, true))).start();

	}

}
