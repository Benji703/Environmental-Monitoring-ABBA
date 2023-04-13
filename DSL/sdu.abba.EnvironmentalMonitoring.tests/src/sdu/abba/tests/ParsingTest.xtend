/*
 * generated by Xtext 2.30.0
 */
package sdu.abba.tests

import com.google.inject.Inject
import org.eclipse.xtext.testing.InjectWith
import org.eclipse.xtext.testing.extensions.InjectionExtension
import org.eclipse.xtext.testing.util.ParseHelper
import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.^extension.ExtendWith
import sdu.abba.environmentalMonitoring.BatchSize
import sdu.abba.environmentalMonitoring.Day
import sdu.abba.environmentalMonitoring.Hour
import sdu.abba.environmentalMonitoring.Minute
import sdu.abba.environmentalMonitoring.Model
import sdu.abba.environmentalMonitoring.SamplingRate
import sdu.abba.environmentalMonitoring.Second
import sdu.abba.environmentalMonitoring.Sensor
import sdu.abba.environmentalMonitoring.Unit

@ExtendWith(InjectionExtension)
@InjectWith(EnvironmentalMonitoringInjectorProvider)
class ParsingTest {
	@Inject ParseHelper<Model> parseHelper
	
	@Test
	def void loadModel() {
		val result = parseHelper.parse('''
			machine MachineA
			  sensor SensorA
			    sampling rate 	= 5/s
			    batch size 		= 100
		''')
		Assertions.assertNotNull(result)
		val errors = result.eResource.errors
		Assertions.assertTrue(errors.isEmpty, '''Unexpected errors: «errors.join(", ")»''')
	}
	
	@Test
	def void machine() {
		val result = parseHelper.parse('''
			machine MachineA
		''')
		Assertions.assertNotNull(result)
		
		Assertions.assertTrue(result.machines.size == 1, "Incorrect number of machines")
		Assertions.assertTrue(result.machines.get(0).name == "MachineA", "Incorrect ID for machine")
	}
	
	@Test
	def void multipleMachines() {
		val result = parseHelper.parse('''
			machine MachineA
			machine MachineB
		''')
		Assertions.assertNotNull(result)
		
		Assertions.assertTrue(result.machines.size == 2, "Incorrect number of machines")
	}
	
	@Test
	def void sensor() {
		val result = parseHelper.parse('''
			machine MachineA
				sensor SensorA
		''')
		Assertions.assertNotNull(result)
		
		Assertions.assertTrue(result.machines.get(0).sensors.size == 1, "Incorrect number of sensors")
		Assertions.assertEquals("SensorA", result.machines.get(0).sensors.get(0).name, "Incorrect ID for sensor")
	}
	
	@Test
	def void sensorWithBatchSize() {
		val result = parseHelper.parse('''
			machine MachineA
				sensor SensorA
					batch size = 300
		''')
		Assertions.assertNotNull(result)
		
		val batchSizes = result.machines.get(0).sensors.get(0).settings.filter(BatchSize)
		Assertions.assertEquals(1, 		batchSizes.size, 			"Incorrect number of batch sizes")
		Assertions.assertEquals(300, 	batchSizes.get(0).value, 	"Incorrect batch size value")
	}
	
	@Test
	def void sensorWithSamplingRate() {
		val result = parseHelper.parse('''
			machine MachineA
				sensor SensorA
					sampling rate 	= 100 / s
		''')
		Assertions.assertNotNull(result)
		
		val sampleRates = result.machines.get(0).sensors.get(0).settings.filter(SamplingRate)
		Assertions.assertEquals(1, 		sampleRates.size, 			"Incorrect number of sample rates")
		Assertions.assertEquals(100, 	sampleRates.get(0).value, 	"Incorrect sample rate value")
	}
	
	@Test
	def void sensorWithSamplingRateAndBatchSize() {
		val result = parseHelper.parse('''
			machine MachineA
				sensor SensorA
					sampling rate 	= 100 / s
					batch size 		= 300
		''')
		Assertions.assertNotNull(result)
		
		val settings = result.machines.get(0).sensors.get(0).settings
		Assertions.assertEquals(2, 		settings.size, 				"Incorrect number of settings")
		
		val sampleRates = settings.filter(SamplingRate)
		Assertions.assertEquals(1, 		sampleRates.size, 			"Incorrect number of sample rates")
		Assertions.assertEquals(100, 	sampleRates.get(0).value, 	"Incorrect sample rate value")
		
		val batchSizes = settings.filter(BatchSize)
		Assertions.assertEquals(1, 		batchSizes.size, 			"Incorrect number of batch sizes")
		Assertions.assertEquals(300, 	batchSizes.get(0).value, 	"Incorrect batch size value")
	}
	
	@Test
	def void multipleSensors() {
		val result = parseHelper.parse('''
			machine MachineA
				sensor SensorA
				sensor SensorB
		''')
		Assertions.assertNotNull(result)
		
		Assertions.assertEquals(2, result.machines.get(0).sensors.size, "Incorrect number of machines")
	}
	
	@Test
	def void sampleRatesHaveCorrectUnits() {
		val result = parseHelper.parse('''
			machine MachineA
				sensor SensorA
					sampling rate	= 1 / s
				sensor SensorB
					sampling rate	= 1 / m
				sensor SensorC
					sampling rate	= 1 / h
				sensor SensorD
					sampling rate	= 1 / d
		''')
		Assertions.assertNotNull(result)
		
		val sensors = result.machines.get(0).sensors
		
		Assertions.assertInstanceOf(Second, sensors.get(0).sampleRateUnit, "Not an instance of second")
		Assertions.assertInstanceOf(Minute, sensors.get(1).sampleRateUnit, "Not an instance of minute")
		Assertions.assertInstanceOf(Hour, sensors.get(2).sampleRateUnit, "Not an instance of hour")
		Assertions.assertInstanceOf(Day, sensors.get(3).sampleRateUnit, "Not an instance of day")
	}
	
	def static Unit getSampleRateUnit(Sensor sensor) {
		return sensor.settings.filter(SamplingRate).get(0).unit
	}
}