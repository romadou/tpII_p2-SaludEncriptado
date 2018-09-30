class ApplicationController < ActionController::Base
  protect_from_forgery with: :exception

  def home
    @measures_count = Measure.all.count
    @patients_count = Patient.joins(:measures).distinct.count
    @sensors_count = Sensor.all.count
  end
end
