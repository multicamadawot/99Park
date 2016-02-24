package br.edu.ifba.bsi.embarcados.webservice.sensores;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;

public class LeitorSensoriamento implements Runnable {

	private static final int DESLOCAMENTO_RFID  = 24;
	private static final int DESLOCAMENTO_TAG = 16;
	private static final int DESLOCAMENTO_ESTA = 8;
	
	// referencia/acesso estatico a sensores
	private static Integer sensores = 0;

	public static int getSensores() {
		synchronized (sensores) {
			return sensores;
		}
	}
	
	public static int getRFID() {
		int id = getSensores();
		
		id = id >> DESLOCAMENTO_RFID;
		
		return id;
	}
	
	public static int getTag() {
		int tag = getSensores();
		
		tag = (tag & 16777215) >> DESLOCAMENTO_TAG;
		
		return tag;
	}
	
	public static int getStatus() {
		int status = getSensores();
		
		status = (status & 65535) >> DESLOCAMENTO_ESTA;
		
		return status;
	}
	
	public static int getDistancia() {
		int distancia = getSensores();
		
		distancia = (distancia & 255);
		
		return distancia;
	}

	// acesso a arquivo PIPE de sensoriamento
	private static final String ARQUIVO_PIPE = "/home/acer/Embarcado/embarcados-ir/unidadeiii/workspacecpp/sensoriamento/sensor";
	private RandomAccessFile fifo = null;

	private boolean continuar = true;

	public LeitorSensoriamento() {
		try {
			fifo = new RandomAccessFile(ARQUIVO_PIPE, "r");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void run() {
		continuar = true;
		while (continuar) {
			String s = "";
			try {
				if (((s = fifo.readLine()) != null) && !s.equals("")) {
					synchronized (sensores) {
						sensores = Integer.parseInt(s);
					}

					Thread.sleep(1000);
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		try {
			fifo.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void parar() {
		continuar = false;
	}
	
}
