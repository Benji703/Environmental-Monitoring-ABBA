/*
 * generated by Xtext 2.30.0
 */
package sdu.abba.ide;

import com.google.inject.Guice;
import com.google.inject.Injector;
import org.eclipse.xtext.util.Modules2;
import sdu.abba.EnvironmentalMonitoringRuntimeModule;
import sdu.abba.EnvironmentalMonitoringStandaloneSetup;

/**
 * Initialization support for running Xtext languages as language servers.
 */
public class EnvironmentalMonitoringIdeSetup extends EnvironmentalMonitoringStandaloneSetup {

	@Override
	public Injector createInjector() {
		return Guice.createInjector(Modules2.mixin(new EnvironmentalMonitoringRuntimeModule(), new EnvironmentalMonitoringIdeModule()));
	}
	
}
