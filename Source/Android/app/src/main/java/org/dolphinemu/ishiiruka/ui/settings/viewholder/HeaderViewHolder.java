package org.dolphinemu.ishiiruka.ui.settings.viewholder;

import android.view.View;
import android.widget.TextView;

import org.dolphinemu.ishiiruka.R;
import org.dolphinemu.ishiiruka.model.settings.view.SettingsItem;
import org.dolphinemu.ishiiruka.ui.settings.SettingsAdapter;

public final class HeaderViewHolder extends SettingViewHolder
{
	private TextView mHeaderName;

	public HeaderViewHolder(View itemView, SettingsAdapter adapter)
	{
		super(itemView, adapter);
		itemView.setOnClickListener(null);
	}

	@Override
	protected void findViews(View root)
	{
		mHeaderName = (TextView) root.findViewById(R.id.text_header_name);
	}

	@Override
	public void bind(SettingsItem item)
	{
		mHeaderName.setText(item.getNameId());
	}

	@Override
	public void onClick(View clicked)
	{
		// no-op
	}
}
