module Api::V1
  class SensorsController < ApiController
    before_action :set_sensor, only: [:show, :update, :destroy]

    # GET /api/v1/sensors
    def index
      render json: Sensor.all.order(created_at: :desc)
    end

    # GET /api/v1/sensors/:id
    def show
      render json: @sensor
    end

    # POST /api/v1/sensors
    def create
      sensor = Sensor.new(sensor_params)
      if sensor.save
        render json: sensor, status: :ok
      else
        render json: sensor.errors, status: :unprocessable_entity
      end
    end

    # PUT /api/v1/sensors
    def update
      if @sensor.update_attributes(sensor_params)
        render json: @sensor, status: :ok
      else
        render json: @sensor.errors, status: :unprocessable_entity
      end
    end

    # DESTROY /api/v1/sensors/:id
    def destroy
      render json: @sensor.destroy
    end

    private

    def set_sensor
      @sensor = Sensor.find params[:id]
    end

    def sensor_params
      params.require(:sensor).permit(:name, :reads, :description)
    end
  end
end