class SensorsController < ApplicationController
  before_action :set_sensor, only: [:edit, :update, :destroy]
  def index
    @sensors = Sensor.all.order(:created_at)
  end

  def new
    @sensor = Sensor.new
  end

  def edit
  end

  def create
    @sensor = Sensor.new(sensor_params)
    if @sensor.save
      redirect_to sensors_path, notice: "Sensor creado correctamente"
    else
      render :new
    end
  end

  def update
    if @sensor.update(sensor_params)
      redirect_to sensors_path, notice: "Sensor actualizado correctamente"
    else
      render :edit
    end
  end

  def destroy
    if @sensor.destroy!
      redirect_to sensors_path, notice: "Sensor eliminado correctamente"
    else
      redirect_to sensors_path, alert: "No se pudo eliminar el sensor #{@sensor.name} con ID #{@sensor.id}"
    end
  end

  private

  def sensor_params
    params.require(:sensor).permit(:name, :reads, :description)
  end

  def set_sensor
    @sensor = Sensor.find params[:id]
  end
end
