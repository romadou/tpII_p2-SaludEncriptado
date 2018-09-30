class PatientsController < ApplicationController
  before_action :set_patient, only: [:show, :edit, :update, :destroy]
  def index
    @patients = Patient.all.order(:created_at)
  end

  def show
  end

  def new
    @patient = Patient.new
  end

  def edit
  end

  def create
    @patient = Patient.new(patient_params)
    if @patient.save
      redirect_to @patient, notice: "El paciente fue creado correctamente"
    else
      render :new
    end
  end

  def update
    if @patient.update(patient_params)
      redirect_to @patient, notice: "El paciente fue actualizado correctamente"
    else
      render :edit
    end
  end

  def destroy
    if @patient.destroy
      redirect_to patients_path, notice: "El paciente fue eliminado correctamente"
    else
      redirect_to @patient, alert: "No se pudo eliminar el paciente"
    end
  end

  private

  def set_patient
    @patient = Patient.find params[:id]
  end

  def patient_params
    params.require(:patient).permit(:name, :description, :email, :phone)
  end
end
