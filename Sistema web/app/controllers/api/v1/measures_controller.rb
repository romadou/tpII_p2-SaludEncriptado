module Api::V1
  class MeasuresController < ApiController
    before_action :set_measure, only: [:show, :destroy]

    # GET /api/v1/measures
    def index
      render json: Measure.all.order(created_at: :desc)
    end

    # GET /api/v1/measures/:id
    def show
      render json: @measure
    end

    # POST /api/v1/measures
    def create
      measure = Measure.new(measure_params)
      if measure.save
        render json: measure, status: :ok
      else
        render json: measure.errors, status: :unprocessable_entity
      end
    end

    # DESTROY /api/v1/measures/:id
    def destroy
      render json: @measure.destroy
    end

    private

    def set_measure
      @measure = Measure.find params[:id]
    end

    def measure_params
      params.require(:measure).permit(:value, :sensor_id, :patient_id)
    end
  end
end