/*
 * generated by Xtext 2.30.0
 */
package sdu.abba.validation;

import java.util.HashMap;
import java.util.HashSet;

import org.eclipse.emf.common.util.EList;
import org.eclipse.xtext.validation.Check;

import sdu.abba.environmentalMonitoring.BatchSize;
import sdu.abba.environmentalMonitoring.Binding;
import sdu.abba.environmentalMonitoring.EnvironmentalMonitoringPackage;
import sdu.abba.environmentalMonitoring.Expression;
import sdu.abba.environmentalMonitoring.Machine;
import sdu.abba.environmentalMonitoring.Model;
import sdu.abba.environmentalMonitoring.SamplingRate;
import sdu.abba.environmentalMonitoring.Sensor;
import sdu.abba.environmentalMonitoring.SensorInstantiation;
import sdu.abba.environmentalMonitoring.SensorInstantiationInner;
import sdu.abba.environmentalMonitoring.SensorReference;
import sdu.abba.environmentalMonitoring.Setting;
import sdu.abba.generator.EnvironmentalMonitoringGenerator;

/**
 * This class contains custom validation rules. 
 *
 * See https://www.eclipse.org/Xtext/documentation/303_runtime_concepts.html#validation
 */
public class EnvironmentalMonitoringValidator extends AbstractEnvironmentalMonitoringValidator {
	
	@Check
	public void checkSettingsAreNoteDuplicated(SensorInstantiationInner sensorInner) {
		
		HashMap<String, Integer> settingsCount = new HashMap<>();
		
		for (Setting setting : sensorInner.getSettings()) {
			
			String className = setting.getClass().toString();
			
			settingsCount.putIfAbsent(className, 0);
			int count = settingsCount.get(className);
			count++;
			settingsCount.replace(className, count);
		}
		
		EList<Setting> settings = sensorInner.getSettings();
		for (int i = 0; i < settings.size(); i++) {
			Setting setting = settings.get(i);
			
			String className = setting.getClass().toString();
			Integer count = settingsCount.get(className);
			
			if (count > 1) {
				error("'"+className + "' can only be configured once", EnvironmentalMonitoringPackage.Literals.SENSOR_INSTANTIATION_INNER__SETTINGS, i);
			}
		}
	}
	
	public static String DUPLICATE_MACHINE_NAMES = "duplicateMachineNames";
	
	
	@Check
	public void checkMachineNamesAreNotDuplicated(Model model) {
	
		HashSet<String> names = new HashSet<>();
		
		EList<Machine> machines = model.getMachines();
		for (int i = 0; i < machines.size(); i++) {
			Machine machine = machines.get(i);
			
			if (names.contains(machine.getName())) {
				error("This machine name is already in use", EnvironmentalMonitoringPackage.Literals.MODEL__MACHINES, i, DUPLICATE_MACHINE_NAMES);
			}
			
			names.add(machine.getName());
		}
	}
	
	@Check
	public void checkSensorNamesAreNotDuplicated(Machine machine) {
		
		HashSet<String> names = new HashSet<>();
		
		EList<Sensor> sensors = machine.getSensors();
		for (int i = 0; i < sensors.size(); i++) {
			Sensor sensor = sensors.get(i);
			
			if (names.contains(getName(sensor))) {
				error("This sensor name is already in use", EnvironmentalMonitoringPackage.Literals.MACHINE__SENSORS, i);
			}
			
			names.add(getName(sensor));
		}
		
	}
	
	private String getName(Sensor sensor) {		// TODO: Can actually use the generated Java code from the generator
		
		if (sensor instanceof SensorInstantiation) {
			SensorInstantiation sensorInstantiation = (SensorInstantiation) sensor;
			return sensorInstantiation.getName();
			
		}
		
		if (sensor instanceof SensorReference) {
			SensorReference sensorReference = (SensorReference) sensor;
			return sensorReference.getRef().getName();
		}
		
		throw new UnsupportedOperationException("The type of sensor is not recognized");
		
	}
	
	@Check
	public void checkBindingNamesAreNotDuplicated(Model model) {
		
		HashSet<String> names = new HashSet<>();
		
		EList<Binding> bindings = model.getVariables();
		for (int i = 0; i < bindings.size(); i++) {
			Binding binding = bindings.get(i);
			
			if (names.contains(binding.getName())) {
				error("This variable name is already in use", EnvironmentalMonitoringPackage.Literals.MODEL__VARIABLES, i);
			}
			
			names.add(binding.getName());
		}
		
	}
	
	@Check
	public void checkPinsAreNotDuplicated(Machine machine) {
		
		HashSet<Integer> pins = new HashSet<>();
		
		EList<Sensor> sensors = machine.getSensors();
		for (int i = 0; i < sensors.size(); i++) {
			Sensor sensor = sensors.get(i);
			
			if (pins.contains(getPin(sensor))) {
				error("The pin is already in use", EnvironmentalMonitoringPackage.Literals.MACHINE__SENSORS, i);
			}
			
			pins.add(getPin(sensor));
		}
		
	}

	private Integer getPin(Sensor sensor) {
		
		Expression expression = null;
		
		if (sensor instanceof SensorInstantiation) {
			SensorInstantiation sensorInstantiation = (SensorInstantiation) sensor;
			expression = sensorInstantiation.getInner().getPin();
			
		}
		else if (sensor instanceof SensorReference) {
			SensorReference sensorReference = (SensorReference) sensor;
			expression = sensorReference.getRef().getSensor().getPin();
		}
		
		if (expression == null) {
			throw new UnsupportedOperationException("The type of sensor is not recognized");
		}
		
		return EnvironmentalMonitoringGenerator.compute(expression);	
	}
	
	@Check
	public void checkBothSensorSettingsAreSet(SensorInstantiationInner sensorInner) {
		
		boolean isBatchSizeSet 		= false;
		boolean isSamplingRateSet 	= false;
		
		EList<Setting> settings = sensorInner.getSettings();
		for (int i = 0; i < settings.size(); i++) {
			Setting setting = settings.get(i);
			
			if (setting instanceof BatchSize) {
				isBatchSizeSet = true;
				continue;
			}
			
			if (setting instanceof SamplingRate) {
				isSamplingRateSet = true;
				continue;
			}
		}
		
		if (!isBatchSizeSet) {
			error("The batch size is missing", EnvironmentalMonitoringPackage.Literals.SENSOR_INSTANTIATION_INNER__SETTINGS);
		}
		
		if (!isSamplingRateSet) {
			error("The sampling rate is missing", EnvironmentalMonitoringPackage.Literals.SENSOR_INSTANTIATION_INNER__SETTINGS);
		}
		
	}
	
}
