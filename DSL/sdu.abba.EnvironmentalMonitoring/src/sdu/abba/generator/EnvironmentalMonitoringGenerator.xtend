/*
 * generated by Xtext 2.30.0
 */
package sdu.abba.generator

import java.io.InvalidObjectException
import java.net.ConnectException
import java.net.URI
import java.net.http.HttpClient
import java.net.http.HttpRequest
import java.net.http.HttpResponse.BodyHandlers
import java.util.HashMap
import java.util.Map
import org.eclipse.emf.ecore.resource.Resource
import org.eclipse.xtext.generator.AbstractGenerator
import org.eclipse.xtext.generator.IFileSystemAccess2
import org.eclipse.xtext.generator.IGeneratorContext
import sdu.abba.environmentalMonitoring.BatchSize
import sdu.abba.environmentalMonitoring.Day
import sdu.abba.environmentalMonitoring.Division
import sdu.abba.environmentalMonitoring.Expression
import sdu.abba.environmentalMonitoring.Hour
import sdu.abba.environmentalMonitoring.Machine
import sdu.abba.environmentalMonitoring.Minute
import sdu.abba.environmentalMonitoring.Model
import sdu.abba.environmentalMonitoring.Multiplication
import sdu.abba.environmentalMonitoring.Number
import sdu.abba.environmentalMonitoring.Plus
import sdu.abba.environmentalMonitoring.SamplingRate
import sdu.abba.environmentalMonitoring.Second
import sdu.abba.environmentalMonitoring.Sensor
import sdu.abba.environmentalMonitoring.Setting
import sdu.abba.environmentalMonitoring.Subtraction
import sdu.abba.environmentalMonitoring.VariableBinding
import sdu.abba.environmentalMonitoring.VariableReference
import sdu.abba.environmentalMonitoring.SensorInstantiation
import sdu.abba.environmentalMonitoring.SensorReference
import org.eclipse.emf.common.util.EList

/**
 * Generates code from your model files on save.
 * 
 * See https://www.eclipse.org/Xtext/documentation/303_runtime_concepts.html#code-generation
 */
class EnvironmentalMonitoringGenerator extends AbstractGenerator {
	
	val httpClient = HttpClient.newHttpClient();
	static val variableMap = new HashMap<String, Integer>()

	override void doGenerate(Resource resource, IFileSystemAccess2 fsa, IGeneratorContext context) {
		
		val fileName = resource.URI.trimFileExtension().lastSegment();
		
		val model = resource.contents
			.filter(Model)
			.toList
			.get(0)
		val json = serializeToJson(model)
		
		fsa.generateFile(fileName+'.json', json)
		
		// TODO: Move this to a dedicated button in the IDE interface
		sendToBackend(json)
		
	}
	
	private def void sendToBackend(String json) {
		val request = HttpRequest.newBuilder()
		.uri(new URI("http://localhost:8080/config"))
		.header("content-type", "application/json")
		.POST(HttpRequest.BodyPublishers.ofString(
			json
		))
		.build();
			
		try {
			
			val response = httpClient.send(request, BodyHandlers.ofString())
		
			if (response.statusCode != 200) {
				System.out.println(response.body)
				throw new Exception("Not 200 status code was received");
			}
			
		}
		catch (ConnectException e) {
			System.out.println("Could not connect to server");
			// TODO: Show pop up window that explains that there is no connection
		}
	}
		
	def String serializeToJson(Model model) {
		
		model.assignVariables
		
		
		val machines = model.machines
		
		val json = '''
		[
			«FOR machine : machines SEPARATOR ','»
				«machine.serialize»
			«ENDFOR»
		]
		'''
		
		return json
	}
	
	static def Map<String, Integer> assignVariables(Model model) {
		variableMap.clear
		
		model.variables
			.filter(VariableBinding)
			.forEach[assign]
			
		return variableMap
	}
		
	static def void assign(VariableBinding variableBinding) {
		val key = variableBinding.name
		val value = variableBinding.expression.compute
		
		variableMap.put(key, value)
	}
		
	def String serialize(Machine machine)'''
	{
		"name": "«machine.name»",
		"sensors": [
			«FOR sensor : machine.sensors SEPARATOR ','»
				«sensor.serialize»
			«ENDFOR»
		]
	}
	'''
	
	def String serialize(Sensor sensor)'''
	{
		"name": "«sensor.name»",
		"settings": {
			«FOR setting : sensor.settings SEPARATOR ','»
				«setting.serializeSetting»
			«ENDFOR»
		}
	}
	'''
		
	def EList<Setting> settings(Sensor sensor) {
		
		switch (sensor) {
			SensorInstantiation: sensor.inner.settings
			SensorReference: sensor.ref.sensor.settings
		}
	}
		
	def String name(Sensor sensor) {
		
		switch (sensor) {
			SensorInstantiation: sensor.name
			SensorReference: sensor.ref.name
		}
		
	}
	
	def dispatch String serializeSetting(Setting setting) {
		throw new UnsupportedOperationException("We don't handle abstract settings")
	}
	
	def dispatch String serializeSetting(BatchSize batchSize)'''
	"batchSize": «batchSize.value.compute»
	'''
	
	def dispatch String serializeSetting(SamplingRate samplingRate)'''
	"samplingRate": «samplingRate.convertToSeconds»
	'''
	
	def float convertToSeconds(SamplingRate samplingRate) {
		val unit = samplingRate.unit
		
		switch unit {
			Second: 	return samplingRate.value.compute
			Minute: 	return samplingRate.value.compute / 60f
			Hour: 		return samplingRate.value.compute / (60f*60)
			Day: 		return samplingRate.value.compute / (60f*60*24)
			default: 	throw new UnsupportedOperationException("Unit type is not supported")
		}
	}
//	
//	def static makeAssignment(VariableBinding assignment) {
//		val value = assignment.expression.computeExp()
//		variables.put(assignment.name, value)
//	}
	
	
	def static dispatch int compute(Number number) {
		return number.value
	}
//	
//	def static dispatch int computeExp(VariableUse varRef) {
//		val binding = varRef.ref
//		var name = ""
//		
//		switch binding {
//			VariableBinding: 	name = binding.name
//			LetBinding:			name = binding.name
//		}
//		
//		return variables.get(name)
//	}
	
	def static dispatch int compute(Expression expression) {
		throw new InvalidObjectException("This type of expression is not known"); // TODO: Make this more proper
	}
	
	def static dispatch int compute(Plus plus) {
		return plus.left.compute + plus.right.compute
	}
	
	def static dispatch int compute(Subtraction subtraction) {
		return subtraction.left.compute - subtraction.right.compute
	}
	
	def static dispatch int compute(Multiplication multiplication) {
		return multiplication.left.compute * multiplication.right.compute
	}
	
	def static dispatch int compute(Division division) {
		return division.left.compute / division.right.compute
	}
	
	def static dispatch int compute(VariableReference varRef) {
		val name = varRef.ref.name
		return variableMap.get(name)
	}
}
