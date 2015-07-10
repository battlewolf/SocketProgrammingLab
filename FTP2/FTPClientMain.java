/**
 * 
 */

/**
 * @author vinay-3264
 *
 */

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

//import org.omg.CORBA.portable.OutputStream;

/**
 * 
 */

/**
 * @author vinay-3264
 *
 */

//The ftpclient is single threaded 
class FTPClient implements Runnable {
	private Socket clientSocket = null;
	private Scanner consoleScanner = new Scanner (System.in);
	private String hostName = null;
	private ObjectOutputStream out = null;
	private ObjectInputStream in = null;

	private int port;

	FTPClient(String hostName, int port) {
		this.hostName = hostName;
		this.port = port;
	}

	private void initSocket() {
		try {
			System.out.println("Trying to connect to host "+hostName);
			if (hostName != null) {
				clientSocket = new Socket(InetAddress.getByName(hostName),port);
			} else {
				throw new RuntimeException("Error in creating connection. Host name is null");
			}
			System.out.println("Connection established!!!!");
			System.out.println("Client running on port "+clientSocket.getLocalPort());

			out = new ObjectOutputStream(clientSocket.getOutputStream());
			in = new ObjectInputStream(clientSocket.getInputStream());
		}
		catch (UnknownHostException e) {
			e.printStackTrace();
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		catch (IllegalArgumentException e) {
			e.printStackTrace();
		}
	}

	private void closeConnection()  {
		System.out.println("Closing connection....");
		consoleScanner.close();
		try {
			out.close();
		} catch (IOException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		}
		try {
			in.close();
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		try {
			clientSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private void writeFile(File file) {
		FileWriter fileWriter = null;
		FileReader fileReader = null;
		char[] buff = new char[1 << 10]; //The file is read in chunks of 1024 bytes
		try {
			fileReader = new FileReader(file);
			fileWriter = new FileWriter(file);
			int returnValue;
			
			while ((returnValue = fileReader.read(buff)) != -1) {
				fileWriter.write(buff);
			}
			
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		finally {
			try {
				if (fileReader != null)
					fileReader.close();
				if (fileWriter != null)
					fileWriter.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	
	private void listFiles(File[] files) {
		for (File file : files) {
			System.out.println (file.getName());
		}
	}
	
	private Packet processCommand(String command) {
		Packet packet = null;
		command = command.trim();
		String[] string = command.split(" +");
		
		System.out.println(string.length);
		for (int i = 0; i < string.length; ++i)
			System.out.println (string[i]);	
		
		if (!(string.length <= 2 && string.length >= 1)) return packet;
		if (!(string[0].equals("GET") || string[0].equals("LIST") || string[0].equals("EXIT"))) return packet;
		
		if(string[0].equals("GET")) {
			packet = new Packet(Operations.GET, string[1]);
		}
		else if(string[0].equals("LIST")) {
			packet = new Packet(Operations.LIST);
		}
		else if(string[0].equals("EXIT")) {
			packet = new Packet(Operations.EXIT);
		}
		
		return packet;
	}

	private void startLoop() {
		System.out.println("File Sharing session started with server");
		
		boolean exit = false;
		
		while (!exit) {
			System.out.print(">>> ");
			
			
			String lineContent = consoleScanner.nextLine();
			Packet sendPacket = processCommand(lineContent);
			
			
			
			if(sendPacket == null) {
				System.out.println("Invalid Command");
				continue;
			}
			
			
			try {
				//System.out.println(sendOperation.fileName);
				out.writeObject(sendPacket);
			} catch (IOException e1) {
				System.out.println("Error occured during writing object");
				continue;
			}
		/*	catch (NotSerializableException e) {
				System.out.println("The object to be sent can't be serialized");
				continue;
			}*/
			
			switch (sendPacket.operation) {
			case EXIT: { 
				exit = true;
				break;
			}
			case GET: {
				try {
					File file = (File ) in.readObject();
					if (file == null) {
						System.out.println("The file you requested does not exist");
						break;
					}
					
					System.out.println("Fetching file . . .");
					writeFile(file);
					System.out.println("File successfully served . . .");
				} catch (IOException e) {
					e.printStackTrace();
				} catch (ClassNotFoundException e) {
					e.printStackTrace();
				}
				break;
			}
			case LIST: {
				try {
					File[] fileList = (File[] ) in.readObject();
					listFiles(fileList);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (ClassNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				break;
			}
			
			}
			
			
			//The server recies EXIT and the connection is closed
		}
	}

	public void run() {
		initSocket();
		startLoop();
		closeConnection();
	}
}


public class FTPClientMain {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		(new Thread(new FTPClient(args[0], 10000))).start();
	}

}

