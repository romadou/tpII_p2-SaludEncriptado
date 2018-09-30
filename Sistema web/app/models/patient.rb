class Patient < ApplicationRecord
  has_many :measures

  validates :name, :email, presence: true
end
