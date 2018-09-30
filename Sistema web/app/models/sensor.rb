class Sensor < ApplicationRecord
  has_many :measures, dependent: :destroy

  validates :name, :reads, presence: true
end
