package br.edu.ifba.bsi.embarcados.cliente.conector;



import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

@SuppressWarnings("deprecation")
public class Conector {

	private static final String ENDERECO_WS = "http://192.168.0.110:8080/smartpark/v1/sw/";

	public String acessar(String urlFuncao) {
		String resultado = "";

		@SuppressWarnings("resource")
		HttpClient cliente = new DefaultHttpClient();
		HttpGet get = new HttpGet(ENDERECO_WS + urlFuncao);
		HttpResponse resposta;
		try {
			resposta = cliente.execute(get);
			BufferedReader br = new BufferedReader(new InputStreamReader(resposta.getEntity().getContent()));

			resultado = br.readLine();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return resultado;
	}
	
	// Pegando ID 

	public Integer getId() {
		Integer id = 0;

		String sid = acessar("id");
		if (sid != "") {
			id = Integer.parseInt(sid);
		}

		return id;
	}

	public Integer getDistancia() {
		Integer distancia = 0;

		String sbats = acessar("sensores/distancia");
		if (sbats != "") {
			distancia = Integer.parseInt(sbats);
		}

		return distancia;
	}

	public Integer getStatus() {
		Integer status = 0;

		String stemp = acessar("sensores/status");
		if (stemp != "") {
			status = Integer.parseInt(stemp);
		}

		return status;
	}
	
	public Integer getTag() {
		Integer tag = 0;

		String stemp = acessar("sensores/tag");
		if (stemp != "") {
			tag = Integer.parseInt(stemp);
		}

		return tag;
	}

}
