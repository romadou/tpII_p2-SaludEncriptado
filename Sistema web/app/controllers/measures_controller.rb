class MeasuresController < ApplicationController
  before_action :set_measure
  def destroy
    if @measure.destroy
      redirect_to @measure.patient, notice: "La medición fue eliminada correctamente"
    else
      redirect_to @measure.patient, alert: "No se pudo eliminar la medición"
    end
  end

  private

  def set_measure
    @measure = Measure.find params[:id]
  end
end