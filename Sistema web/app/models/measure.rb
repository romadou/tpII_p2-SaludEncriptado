class Measure < ApplicationRecord
  belongs_to :patient
  belongs_to :sensor

  validates :value, :patient, :sensor, presence: true

  def decrypted_value
    # The value should be in base 64
    # If value is plain encrypted append { format: :plain }
    # AES.decrypt(self[:value], 'aaaabbbbccccdddd', { format: :plain })

    # Decode base 64
    Base64.decode64(self[:value]).unpack("A*").first.to_i
  end
end
