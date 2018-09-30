class ChangeValueToStringInMeasures < ActiveRecord::Migration[5.1]
  def change
    change_column :measures, :value, :text
  end
end
