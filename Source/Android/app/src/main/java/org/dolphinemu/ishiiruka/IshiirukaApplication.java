package org.dolphinemu.ishiiruka;

import android.app.Application;

import org.dolphinemu.ishiiruka.model.GameDatabase;
import org.dolphinemu.ishiiruka.services.DirectoryInitializationService;
import org.dolphinemu.ishiiruka.utils.PermissionsHandler;

public class IshiirukaApplication extends Application
{
	public static GameDatabase databaseHelper;

	@Override
	public void onCreate()
	{
		super.onCreate();

		if (PermissionsHandler.hasWriteAccess(getApplicationContext()))
			DirectoryInitializationService.startService(getApplicationContext());

		databaseHelper = new GameDatabase(this);
	}
}
