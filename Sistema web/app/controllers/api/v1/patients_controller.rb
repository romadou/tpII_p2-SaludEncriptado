module Api::V1
  class PatientsController < ApiController
    before_action :set_patient, only: [:show, :update, :destroy]

    # GET /api/v1/patients
    def index
      render json: Patient.all.order(created_at: :desc)
    end

    # GET /api/v1/patients/:id
    def show
      render json: @patient
    end

    # POST /api/v1/patients
    def create
      patient = Patient.new(patient_params)
      if patient.save
        render json: patient, status: :ok
      else
        render json: patient.errors, status: :unprocessable_entity
      end
    end

    # PUT /api/v1/patients
    def update
      if @patient.update_attributes(patient_params)
        render json: @patient, status: :ok
      else
        render json: @patient.errors, status: :unprocessable_entity
      end
    end

    # DESTROY /api/v1/patients/:id
    def destroy
      render json: @patient.destroy
    end

    private

    def set_patient
      @patient = Patient.find params[:id]
    end

    def patient_params
      params.require(:patient).permit(:name, :description, :email, :phone)
    end
  end
end