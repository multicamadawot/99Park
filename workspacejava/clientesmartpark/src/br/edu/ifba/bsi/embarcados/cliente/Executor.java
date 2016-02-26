package br.edu.wot.bsi.embarcados.cliente;

import br.edu.wot.embarcados.cliente.conector.Conector;

public class Executor {

	public static void main(String[] args) throws InterruptedException {
		Conector conector = new Conector();

		while (true) {
			Integer id = conector.getId();
			System.out.println("id = " + id);
			
			System.out.println("Sensores");
			
			Integer distancia = conector.getDistancia();
			System.out.println("distancia = " + distancia);
			
			Integer status = conector.getStatus();
			System.out.println("status = " + status);
			
			Integer tag = conector.getTag();
			System.out.println("Id tag = " + tag);
		
			System.out.println("Final de leitura!");
			System.out.println("...");
				
			Thread.sleep(1000);
		}
	}

}
