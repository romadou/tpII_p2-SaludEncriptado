class CreateMeasures < ActiveRecord::Migration[5.1]
  def change
    create_table :measures do |t|
      t.references :patient, foreign_key: true
      t.references :sensor, foreign_key: true
      t.integer :value

      t.timestamps
    end
  end
end
