package br.edu.ifba.bsi.embarcados.webservice;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

import br.edu.ifba.bsi.embarcados.webservice.sensores.LeitorSensoriamento;

@Path("sw")
public class Servico {

	@GET
	@Path("/id/")
	@Produces(MediaType.TEXT_PLAIN)
	public String getId() {
		return LeitorSensoriamento.getRFID() + "";
	}

	@GET
	@Path("/sensores")
	@Produces(MediaType.TEXT_PLAIN)
	public String getSensores() {
		return LeitorSensoriamento.getSensores() + "";
	}

	@GET
	@Path("/sensores/tag")
	@Produces(MediaType.TEXT_PLAIN)
	public String getMovimento() {
		return LeitorSensoriamento.getTag() + "";
	}

	@GET
	@Path("/sensores/status")
	@Produces(MediaType.TEXT_PLAIN)
	public String getBatimentos() {
		return LeitorSensoriamento.getStatus() + "";
	}

	@GET
	@Path("/sensores/distancia")
	@Produces(MediaType.TEXT_PLAIN)
	public String getTemperatura() {
		return LeitorSensoriamento.getDistancia() + "";
	}
	
}
