# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20171105030435) do

  # These are extensions that must be enabled in order to support this database
  enable_extension "plpgsql"

  create_table "measures", force: :cascade do |t|
    t.bigint "patient_id"
    t.bigint "sensor_id"
    t.text "value"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.index ["patient_id"], name: "index_measures_on_patient_id"
    t.index ["sensor_id"], name: "index_measures_on_sensor_id"
  end

  create_table "patients", force: :cascade do |t|
    t.string "name"
    t.string "description"
    t.string "email"
    t.string "phone"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "sensors", force: :cascade do |t|
    t.string "name"
    t.string "reads"
    t.string "description"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  add_foreign_key "measures", "patients"
  add_foreign_key "measures", "sensors"
end
