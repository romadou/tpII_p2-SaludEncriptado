class PatientSerializer < ActiveModel::Serializer
  attributes :id,
             :name,
             :description,
             :email,
             :phone,
             :created_at
end
