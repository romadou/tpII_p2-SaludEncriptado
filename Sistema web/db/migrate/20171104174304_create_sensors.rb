class CreateSensors < ActiveRecord::Migration[5.1]
  def change
    create_table :sensors do |t|
      t.string :name
      t.string :reads
      t.string :description

      t.timestamps
    end
  end
end
